/*
RfSSAO - Implementation.
*/
#include "rf_ssao_gl.h"
#include "zpd.h"

#include <random>

#include <glm/gtc/type_ptr.hpp>

#include "rf_state_gl.h"
#include "rf_geometry_buffer_gl.h"
#include "rf_compositor_gl.h"
#include "rf_quad_buffer_gl.h"

using namespace zootopia;

// SINGLE-TONE CLASS
static RfSSAOGL* _ssao = nullptr;

RfSSAOGL::RfSSAOGL()
{
    glGenFramebuffers(1, &_ssaoFBO);
    glGenFramebuffers(1, &_ssaoBlurFBO);

    glGenTextures(1, &_ssaoColorBuffer);
    glGenTextures(1, &_ssaoBlurColorBuffer);
    glGenTextures(1, &_noiseTexture);

    _ssaoShader = new RfShaderGL("shader/glsl/ssao.vert", "shader/glsl/ssao.frag");
    _ssaoBlurShader = new RfShaderGL("shader/glsl/ssao.vert", "shader/glsl/ssao_blur.frag");

    _quadBuffer = RfQuadBufferGL::getBuffer();

    RF_GL_CHECK_ERROR();
}


RfSSAOGL::~RfSSAOGL()
{
    glDeleteTextures(1, &_ssaoColorBuffer);
    glDeleteTextures(1, &_ssaoBlurColorBuffer);
    glDeleteTextures(1, &_noiseTexture);

    glDeleteFramebuffers(1, &_ssaoFBO);
    glDeleteFramebuffers(1, &_ssaoBlurFBO);

    ZDELETEZ_SAFE(_ssaoShader);
    ZDELETEZ_SAFE(_ssaoBlurShader);

    RF_GL_CHECK_ERROR();
}


RfSSAOGL* RfSSAOGL::getInstance()
{
    if (_ssao) {
        return _ssao;
    }
    else {
        _ssao = new RfSSAOGL;
        return _ssao;
    }
}

void RfSSAOGL::initialize(const RfSize & fboSize)
{
    const GLsizei width = RfScalarTruncToInt(fboSize.w);
    const GLsizei height = RfScalarTruncToInt(fboSize.h);

    // Set Samplers
    _ssaoShader->use();
    glUniform1i(glGetUniformLocation(_ssaoShader->getShaderProgObj(), "gPositionDepth"), 0);
    glUniform1i(glGetUniformLocation(_ssaoShader->getShaderProgObj(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(_ssaoShader->getShaderProgObj(), "texNoise"), 2);

    _ssaoBlurShader->use();
    glUniform1i(glGetUniformLocation(_ssaoBlurShader->getShaderProgObj(), "ssaoInput"), 0);

    // - SSAO color buffer
    glBindFramebuffer(GL_FRAMEBUFFER, _ssaoFBO);
    glBindTexture(GL_TEXTURE_2D, _ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _ssaoColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        ZLOG_E("SSAO Framebuffer not complete!");

    // - and blur stage
    glBindFramebuffer(GL_FRAMEBUFFER, _ssaoBlurFBO);
    glBindTexture(GL_TEXTURE_2D, _ssaoBlurColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _ssaoBlurColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        ZLOG_E("SSAO Blur Framebuffer not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    // Lamda expression.
    auto lerp = [](GLfloat a, GLfloat b, GLfloat f) -> GLfloat { return a + f * (b - a); };

    // Sample kernel
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    for (GLuint i = 0; i < 64; ++i)
    {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        GLfloat scale = GLfloat(i) / 64.0;

        // Scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        _ssaoKernel.push_back(sample);
    }

    // Noise texture
    std::vector<glm::vec3> ssaoNoise;
    for (GLuint i = 0; i < 16; i++)
    {
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        ssaoNoise.push_back(noise);
    }
    glBindTexture(GL_TEXTURE_2D, _noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    RF_GL_CHECK_ERROR();
}


void RfSSAOGL::resize(const RfSize & fboSize)
{
    ZABORT_NOT_IMPLEMENTED();

    RF_GL_CHECK_ERROR();
}


void RfSSAOGL::destroy()
{
    ZDELETEZ_SAFE(_ssao);

    RF_GL_CHECK_ERROR();
}


void RfSSAOGL::draw()
{
    RfGeometryBufferGL* gBuffer = RfGeometryBufferGL::getBuffer();

    // Create SSAO texture
    glBindFramebuffer(GL_FRAMEBUFFER, _ssaoFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    _ssaoShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBuffer->getPositionDepthOutputTextureID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalOutputTextureID());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _noiseTexture);
    // Send kernel + rotation 
    for (GLuint i = 0; i < 64; ++i)
        glUniform3fv(glGetUniformLocation(_ssaoShader->getShaderProgObj(), ("samples[" + std::to_string(i) + "]").c_str()), 1, &_ssaoKernel[i][0]);
    glUniformMatrix4fv(glGetUniformLocation(_ssaoShader->getShaderProgObj(), "projection"), 1, GL_FALSE, glm::value_ptr(RfCompositorGL::_displayCamera->getProjMatrix()));
    _quadBuffer->bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    _quadBuffer->unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // Blur SSAO texture to remove noise
    glBindFramebuffer(GL_FRAMEBUFFER, _ssaoBlurFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    _ssaoBlurShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _ssaoColorBuffer);
    _quadBuffer->bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    _quadBuffer->unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}





