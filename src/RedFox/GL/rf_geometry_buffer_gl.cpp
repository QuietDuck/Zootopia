/*
RfGeometryBufferGL - Implementation
*/
#include "rf_geometry_buffer_gl.h"
#include "zpd.h"

#include "rf_state_gl.h"

using namespace zootopia;

// SINGLE-TONE CLASS
RfGeometryBufferGL* RfGeometryBufferGL::_gBuffer = nullptr;

RfGeometryBufferGL::RfGeometryBufferGL() :
    _gBufferFBO(GL_NONE)
{}

RfGeometryBufferGL::~RfGeometryBufferGL() {}

RfGeometryBufferGL* RfGeometryBufferGL::getBuffer()
{
    if (_gBuffer) {
        return _gBuffer;
    }      
    else {
        _gBuffer = new RfGeometryBufferGL;
        return _gBuffer;
    }
}

void RfGeometryBufferGL::initialize(const RfSize& fboSize)
{
    const GLsizei width = RfScalarTruncToInt(fboSize.w);
    const GLsizei height = RfScalarTruncToInt(fboSize.h);

    glGenFramebuffers(1, &_gBufferFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _gBufferFBO);
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
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    RF_GL_CHECK_ERROR();
}

void RfGeometryBufferGL::resize(const RfSize& fboSize)
{
    RF_GL_CHECK_ERROR();
}

void RfGeometryBufferGL::destroy()
{
    if (_rboDepth NEQ GL_NONE)
        glDeleteRenderbuffers(1, &_rboDepth);
    if (_gPosition NEQ GL_NONE)
        glDeleteTextures(1, &_gPosition);
    if (_gNormal NEQ GL_NONE)
        glDeleteTextures(1, &_gNormal);
    if (_gAlbedoSpec NEQ GL_NONE)
        glDeleteTextures(1, &_gAlbedoSpec);
    if (_gBufferFBO NEQ GL_NONE)
        glDeleteFramebuffers(1, &_gBufferFBO);

    RF_GL_CHECK_ERROR();
}

void RfGeometryBufferGL::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _gBufferFBO);

    RF_GL_CHECK_ERROR();
}

void RfGeometryBufferGL::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    RF_GL_CHECK_ERROR();
}

