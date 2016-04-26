/*
RfCompositorGL - Implementation
*/
#include "rf_compositor_gl.h"
#include "zpd.h"

#include <iostream>

#include "rf_light.h"
#include "rf_point_light.h"

#include "rf_light_manager_gl.h"
#include "rf_point_light_gl.h"
#include "rf_state_gl.h"

using namespace zootopia;

const GLuint DEFAULT_FRAMEBUFFER = 0;

// SINGLE DEFERRED SHADER.
RfShaderGL* RfCompositorGL::_deferredShader = nullptr;
RfShaderGL* RfCompositorGL::_currentShader = nullptr;

RfCameraGL* RfCompositorGL::_displayCamera = nullptr;

RfCompositorGL::RfCompositorGL() :
    _gBuffer(nullptr)
{}

RfCompositorGL::~RfCompositorGL() {}

void RfCompositorGL::initialize(const RfSize& fboSize)
{
    /* Initialize GLEW */
    glewExperimental = GL_TRUE;
    if (glewInit()) {
        ZLOG_E("Unable to initialize GLEW ... exiting.");
    }
    else {
        fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    }
    // TO GET RID OF GLFW ERROR. (ERROR CODE: 1280)
    glGetError();

    /* Initialize DevIL */
    ilInit();

    ///////////////////////////////////////////////////////////////

    _grid = new RfGridGL;
    _quad = new RfQuadGL;
    _deferredShader = new RfShaderGL("shader/glsl/g_buffer.vert", "shader/glsl/g_buffer.frag");

    _gBuffer = RfGeometryBufferGL::getBuffer();
    _gBuffer->initialize(fboSize);

    _lightManager = RfLightManagerGL::getInstance();

    //*
    const GLuint NR_LIGHTS = 64;
    srand(7);
    for (GLuint i = 0; i < NR_LIGHTS; i++) {

        RfPointLight* pointLight = new RfPointLightGL(
            RfPoint3(0, 0, 0),
            RfColor::make_RGBA32(0xFF, 0xFF, 0xFF, 0xFF)
        );

        GLfloat xPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
        GLfloat yPos = ((rand() % 100) / 100.0) * 5.0;
        GLfloat zPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
        pointLight->setPosition(RfPoint3(xPos, yPos, zPos));

        GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5;
        GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5;
        GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5;
        rColor *= 255;
        gColor *= 255;
        bColor *= 255;
        RfColor lightColor = RfColor::make_RGBA32(rColor, gColor, bColor, 0xFF);
        pointLight->setColor(lightColor);
        pointLight->setProperties(0.7f, 1.8f);
    }
    //*/
    /*
    _testLight = new RfPointLightGL(
        RfPoint3(0, 4, 0),
        RfColor::make_RGBA32(0xFF, 0xFF, 0xFF, 0xFF)
    );
    _testLight->setProperties(0.7f/100.0f, 1.8f/100.0f);
    //*/
    //*
    _testLight2 = new RfDirLightGL(
        RfVector3(0, -1, 0),
        RfColor::make_RGBA32(0x1F, 0x1F, 0x1F, 0xFF)
    );
    //*/

    //*
    _testLightR = new RfSpotLightGL(
        RfPoint3(1, 8, 1),
        RfVector3(0, -1, 0),
        RfColor::make_RGBA32(0xFF, 0x00, 0x00, 0xFF)
    );

    _testLightG = new RfSpotLightGL(
        RfPoint3(-1, 8, -1),
        RfVector3(0, -1, 0),
        RfColor::make_RGBA32(0x00, 0xFF, 0x00, 0xFF)
    );

    _testLightB = new RfSpotLightGL(
        RfPoint3(2, 8, -2),
        RfVector3(0, -1, 0),
        RfColor::make_RGBA32(0x00, 0x00, 0xFF, 0xFF)
    );
    //*/
}

void RfCompositorGL::resize(const RfSize & fboSize)
{
    ZABORT_NOT_IMPLEMENTED();

    // Resize FBO and viewport.
    RF_GL_CHECK_ERROR();
}

void RfCompositorGL::destroy()
{
    // DELETE INSTANCES.
    if (_gBuffer) {
        _gBuffer->destroy();
    }
    if (_deferredShader) {
        _deferredShader->destroy();
        ZDELETEZ_SAFE(_deferredShader);
    }
    if (_displayShader) {
        _displayShader->destroy();
        ZDELETEZ_SAFE(_displayShader);
    }
    if (_hitTestShader) {
        _hitTestShader->destroy();
        ZDELETEZ_SAFE(_displayShader);
    }
    if (_displayCamera) {
        ZDELETEZ_SAFE(_displayCamera);
    }
    if (_grid) {
        _grid->destroy();
        ZDELETEZ_SAFE(_grid);
    }
    if (_quad) {
        ZDELETEZ_SAFE(_quad);
    }
    RF_GL_CHECK_ERROR();
}

void RfCompositorGL::prepareFrame(const RfSize& frameSize)
{
    // TODO: Should be moved?
    glViewport(0, 0,
        RfScalarTruncToInt(frameSize.w),
        RfScalarTruncToInt(frameSize.h));

    // IT IS NESSESARY.
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClearStencil(0);

    RF_GL_CHECK_ERROR();
}

void RfCompositorGL::renderDisplay(const std::vector<RfObject*> &objects)
{
    ZASSERT(_deferredShader);
    ZASSERT(_displayCamera);

    // BIND MRT FBO.
    _gBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDisable(GL_BLEND); // IT SHOULD BE CALLED.

    // DRAW GRID. (HELPER)
    //_grid->draw();

    // USE DEFERRED SHADER.
    _deferredShader->use();
    _currentShader = _deferredShader;

    // DRAW OBJECTS.
    for (auto object : objects) {
        //object->rotate(0.5f, RfPoint3(0, 1, 0));
        object->draw();
    }

    _gBuffer->unbind(); // for safety.

    RF_GL_CHECK_ERROR();
}

void RfCompositorGL::renderHitTest()
{
    // render registered hit object?
    RF_GL_CHECK_ERROR();
}

void RfCompositorGL::renderUserInterface()
{
    // render user interface (ui, font, etc...)
    RF_GL_CHECK_ERROR();
}

static RfScalar lightPos = 1.0f;
void RfCompositorGL::postProcess()
{
    ZASSERT(_displayShader);
    ZASSERT(_displayCamera);

    lightPos += 0.05f;
    if (lightPos >= 7.0f)
        lightPos = 1.0f;
    //_testLight->setPosition(RfPoint3(0, lightPos, 0));

    // Render output (deferred rendering)
    glBindFramebuffer(GL_FRAMEBUFFER, DEFAULT_FRAMEBUFFER);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Use shader
    _displayShader->use();
    _currentShader = _displayShader;
    //glUniform1i(glGetUniformLocation(_displayShader->getShaderProgObj(), "gPosition"), 0);
    //glUniform1i(glGetUniformLocation(_displayShader->getShaderProgObj(), "gNormal"), 1);
    //glUniform1i(glGetUniformLocation(_displayShader->getShaderProgObj(), "gAlbedoSpec"), 2);

    // RENDER OUTPUT (QUAD TEXTURE)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _gBuffer->getPositionOutputTextureID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _gBuffer->getNormalOutputTextureID());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _gBuffer->getAlbedoSpecOutputTextureID());

    glUniform3fv(glGetUniformLocation(_displayShader->getShaderProgObj(), "viewPos"), 1, glm::value_ptr(_displayCamera->getPosition()));

    _quad->draw();

    //*
    // Copy content of geometry's depth buffer to default framebuffer's depth buffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _gBuffer->getId());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
    glBlitFramebuffer(0, 0, 1280, 720, 0, 0, 1280, 720, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //*/

    _lightManager->drawLightBulb();

    RF_GL_CHECK_ERROR();
}

void RfCompositorGL::finishFrame()
{
    // unbind?
    RF_GL_CHECK_ERROR();
}

void RfCompositorGL::readHitTestBuffer()
{
    // readHitTestTest
    RF_GL_CHECK_ERROR();
}

void RfCompositorGL::setShader(RfShader* shader)
{
    ZASSERT(shader);

    _displayShader = static_cast<RfShaderGL*>(shader);

    /// TEMPORARY... 
    _displayShader->use();
    glUniform1i(glGetUniformLocation(_displayShader->getShaderProgObj(), "gPosition"), 0);
    glUniform1i(glGetUniformLocation(_displayShader->getShaderProgObj(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(_displayShader->getShaderProgObj(), "gAlbedoSpec"), 2);

    /*
    GLuint block_index = 0;
    block_index = glGetProgramResourceIndex(_displayShader->getShaderProgObj(), GL_SHADER_STORAGE_BLOCK, "shader_data");

    GLuint ssbo_binding_point_index = 2;
    glShaderStorageBlockBinding(_displayShader->getShaderProgObj(), block_index, ssbo_binding_point_index);
    */

    RF_GL_CHECK_ERROR();
}

void RfCompositorGL::setCamera(RfCamera* camera)
{
    ZASSERT(camera);

    _displayCamera = static_cast<RfCameraGL*>(camera);

    RF_GL_CHECK_ERROR();
}

// RenderCube() Renders a 1x1 3D cube in NDC.
GLuint cubeVAO = 0;
GLuint cubeVBO = 0;
void RenderCube()
{
    // Initialize (if necessary)
    if (cubeVAO == 0)
    {
        GLfloat vertices[] = {
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
            // Left face
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // Fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // Render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
