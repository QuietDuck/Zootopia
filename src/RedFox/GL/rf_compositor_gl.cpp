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

    _lightManager = new RfLightManagerGL;

    /*
    struct PointLight {

        RfPoint3 position; uint32 pad1;
        RfVector3 color; uint32 pad2;
        RfScalar linear;
        RfScalar quadratic;
        RfScalar radius;
        uint32 pad3;
    };

    std::vector<PointLight> pointLights;

    const GLuint NR_LIGHTS = 256;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    srand(13);
    for (GLuint i = 0; i < NR_LIGHTS; i++) {

        PointLight pointLight;

        // Calculate slightly random offsets
        GLfloat xPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
        GLfloat yPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
        GLfloat zPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
        //GLfloat xPos = -3.0f;
        //GLfloat yPos = 4.0f;
        //GLfloat zPos = 3.0f;
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        pointLight.position = RfPoint3(xPos, yPos, zPos);
        // Also calculate random color
        GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        //GLfloat rColor = 1.0f;
        //GLfloat gColor = 1.0f;
        //GLfloat bColor = 1.0f;
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
        pointLight.color = RfVector3(rColor, gColor, bColor);

        const GLfloat constant = 1.0f; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const GLfloat linear = 0.7f / 1.0f;
        const GLfloat quadratic = 1.8f / 1.0f;
        pointLight.linear = linear;
        pointLight.quadratic = quadratic;

        const GLfloat lightThreshold = 5.0; // 5 / 256
        const GLfloat maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
        const GLfloat radius = (-linear + static_cast<float>(std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0 / lightThreshold) * maxBrightness)))) / (2 * quadratic);
        pointLight.radius = radius;

        pointLight.pad1 = 0;
        pointLight.pad2 = 0;
        pointLight.pad3 = 0;

        pointLights.push_back(pointLight);
    }

    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PointLight) * pointLights.size(), pointLights.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    //*/

    //*
    const GLuint NR_LIGHTS = 128;
    std::vector<RfLight*> lights;
    srand(13);
    for (GLuint i = 0; i < NR_LIGHTS; i++) {

        RfPointLight* pointLight = new RfPointLightGL(
            RfColor::make_RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
            RfPoint3(0, 0, 0));

        GLfloat xPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
        GLfloat yPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
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

        lights.push_back(pointLight);
    }

    _lightManager->setLights(lights);
    //*/

    RF_GL_CHECK_ERROR();
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

    // IT IS NESSESARY.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

void RfCompositorGL::postProcess()
{  
    ZASSERT(_displayShader);
    ZASSERT(_displayCamera);

    // Render output (deferred rendering)
    glBindFramebuffer(GL_FRAMEBUFFER, DEFAULT_FRAMEBUFFER);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Use shader
    _displayShader->use();
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

void RfCompositorGL::setLights(const std::vector<RfLight*>& lights)
{
    for (auto light : lights) {

        switch (light->getType()) {

        case RfLight::Type::kDirectional: {

            RfDirectionalLight* directionalLight = static_cast<RfDirectionalLight*>(light);
            _directionalLights.push_back(directionalLight);
            break;
        }

        case RfLight::Type::kPoint: {

            RfPointLight* pointLight = static_cast<RfPointLight*>(light);
            _pointLights.push_back(pointLight);
            break;
        }

        case RfLight::Type::kSpot: {

            RfSpotLight* spotLight = static_cast<RfSpotLight*>(light);
            _spotLights.push_back(spotLight);
            break;
        }    

        default:

            ZABORT("Unexpected Process: RfCompositorGL::setLights()");
        }
    }

    RF_GL_CHECK_ERROR();
}
