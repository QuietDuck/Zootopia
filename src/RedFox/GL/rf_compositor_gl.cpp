/*
RfCompositorGL - Implementation
*/
#include "rf_compositor_gl.h"
#include "zpd.h"

#include <iostream>

using namespace zootopia;

// SINGLE DEFERRED SHADER.
RfShaderGL* RfCompositorGL::_deferredShader = nullptr;
RfCameraGL* RfCompositorGL::_displayCamera = nullptr;

RfCompositorGL::RfCompositorGL() :
    _gBuffer(0)
,   _gPosition(0)
,   _gNormal(0)
,   _gAlbedoSpec(0)
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

    //////////////////////////////////////////////////////////////
    // GENERATE FRAME BUFFER (FOR DEFERRED RENDERING)
    //////////////////////////////////////////////////////////////

    const GLsizei width = RfScalarTruncToInt(fboSize.w);
    const GLsizei height = RfScalarTruncToInt(fboSize.h);

    glGenFramebuffers(1, &_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);
    // - Position color buffer
    glGenTextures(1, &_gPosition);
    glBindTexture(GL_TEXTURE_2D, _gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _gPosition, 0);
    // - Normal color buffer
    glGenTextures(1, &_gNormal);
    glBindTexture(GL_TEXTURE_2D, _gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _gNormal, 0);
    // - Color + Specular color buffer
    glGenTextures(1, &_gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, _gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _gAlbedoSpec, 0);

    // - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    // - Create and attach depth buffer (renderbuffer)
    glGenRenderbuffers(1, &_rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, _rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rboDepth);
    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        ZLOG_E("Framebuffer not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //////////////////////////////////////////////////////

    glViewport(0, 0, width, height);
}

void RfCompositorGL::resize(const RfSize & fboSize)
{
    ZABORT_NOT_IMPLEMENTED();

    // Resize FBO and viewport.
}

void RfCompositorGL::destroy()
{
    // DELETE GL BUFFERS.
    if (_rboDepth NEQ GL_NONE)
        glDeleteBuffers(1, &_rboDepth);

    if (_gPosition NEQ GL_NONE)
        glDeleteTextures(1, &_gPosition);

    if (_gNormal NEQ GL_NONE)
        glDeleteTextures(1, &_gNormal);

    if (_gAlbedoSpec NEQ GL_NONE)
        glDeleteTextures(1, &_gAlbedoSpec);

    if (_gBuffer NEQ GL_NONE)
        glDeleteBuffers(1, &_gBuffer);

    // DELETE INSTANCES.
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
}

void RfCompositorGL::renderDisplay(const std::vector<RfObject*> &objects)
{
    ZASSERT(_deferredShader);
    ZASSERT(_displayCamera);

    // BIND MRT FBO.
    glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);
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
}

void RfCompositorGL::renderHitTest()
{
    // render registered hit object?
}

void RfCompositorGL::renderUserInterface()
{
    // render user interface (ui, font, etc...)
}

void RfCompositorGL::postProcess()
{  
    ZASSERT(_displayShader);
    ZASSERT(_displayCamera);

    // Render output (deferred rendering)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Use shader
    _displayShader->use();
    //glUniform1i(glGetUniformLocation(_displayShader->getShaderProgObj(), "gPosition"), 0);
    //glUniform1i(glGetUniformLocation(_displayShader->getShaderProgObj(), "gNormal"), 1);
    //glUniform1i(glGetUniformLocation(_displayShader->getShaderProgObj(), "gAlbedoSpec"), 2);

    // RENDER OUTPUT (QUAD TEXTURE)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _gAlbedoSpec);

    /////////////////////////////////////////////////////

    const GLuint NR_LIGHTS = 3;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    srand(13);
    for (GLuint i = 0; i < NR_LIGHTS; i++) {

        // Calculate slightly random offsets
        //GLfloat xPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
        //GLfloat yPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
        //GLfloat zPos = ((rand() % 100) / 100.0) * 10.0 - 5.0;
        GLfloat xPos = -3.0f;
        GLfloat yPos = 4.0f;
        GLfloat zPos = 3.0f;
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // Also calculate random color
        //GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        //GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        //GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat rColor = 1.0f;
        GLfloat gColor = 1.0f;
        GLfloat bColor = 1.0f;
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }

    for (GLuint i = 0; i < NR_LIGHTS; i++) {

        glUniform3fv(glGetUniformLocation(_displayShader->getShaderProgObj(), ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &lightPositions[i][0]);
        glUniform3fv(glGetUniformLocation(_displayShader->getShaderProgObj(), ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &lightColors[i][0]);
        // Update attenuation parameters and calculate radius
        const GLfloat constant = 1.0f; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const GLfloat linear = 0.7f / 15.0f;
        const GLfloat quadratic = 1.8f / 15.0f;
        glUniform1f(glGetUniformLocation(_displayShader->getShaderProgObj(), ("lights[" + std::to_string(i) + "].Linear").c_str()), linear);
        glUniform1f(glGetUniformLocation(_displayShader->getShaderProgObj(), ("lights[" + std::to_string(i) + "].Quadratic").c_str()), quadratic);
        // Then calculate radius of light volume/sphere
        const GLfloat lightThreshold = 5.0; // 5 / 256
        const GLfloat maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
        const GLfloat radius = (-linear + static_cast<float>(std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0 / lightThreshold) * maxBrightness)))) / (2 * quadratic);
        glUniform1f(glGetUniformLocation(_displayShader->getShaderProgObj(), ("lights[" + std::to_string(i) + "].Radius").c_str()), radius);
    }
    glUniform3fv(glGetUniformLocation(_displayShader->getShaderProgObj(), "viewPos"), 1, glm::value_ptr(_displayCamera->getPosition()));

    /////////////////////////////////////////////////////

    _quad->draw();
}

void RfCompositorGL::finishFrame()
{
    // unbind?
}

void RfCompositorGL::readHitTestBuffer()
{
    // readHitTestTest
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
}

void RfCompositorGL::setCamera(RfCamera* camera)
{
    ZASSERT(camera);

    _displayCamera = static_cast<RfCameraGL*>(camera);
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

            ZLOG_I("");
            ZABORT("Unexpected Process");
        }
    }
}
