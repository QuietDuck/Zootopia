/*
RfGeometryBufferGL - Implementation
*/
#include "rf_geometry_buffer_gl.h"
#include "zpd.h"

#include "rf_state_gl.h"

using namespace zootopia;

// SINGLE-TONE CLASS
static RfGeometryBufferGL* _gBuffer = nullptr;

RfGeometryBufferGL::RfGeometryBufferGL() :
    _gPositionDepth(GL_NONE)
,   _gNormal(GL_NONE)
,   _gAlbedoSpec(GL_NONE)
,   _rboDepth(GL_NONE)
{
    glGenTextures(1, &_gPositionDepth);
    glGenTextures(1, &_gNormal);
    glGenTextures(1, &_gAlbedoSpec);
    glGenRenderbuffers(1, &_rboDepth);

    RF_GL_CHECK_ERROR();
}

RfGeometryBufferGL::~RfGeometryBufferGL()
{
    if (_rboDepth NEQ GL_NONE)
        glDeleteRenderbuffers(1, &_rboDepth);
    if (_gPositionDepth NEQ GL_NONE)
        glDeleteTextures(1, &_gPositionDepth);
    if (_gNormal NEQ GL_NONE)
        glDeleteTextures(1, &_gNormal);
    if (_gAlbedoSpec NEQ GL_NONE)
        glDeleteTextures(1, &_gAlbedoSpec);

    RF_GL_CHECK_ERROR();
}

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

    _gBufferFBO.bind();
    // Position color buffer
    glBindTexture(GL_TEXTURE_2D, _gPositionDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _gPositionDepth, 0);
    // Normal color buffer
    glBindTexture(GL_TEXTURE_2D, _gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _gNormal, 0);
    // Color + Specular color buffer
    glBindTexture(GL_TEXTURE_2D, _gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _gAlbedoSpec, 0);

    // Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    // Create and attach depth buffer (renderbuffer)
    glBindRenderbuffer(GL_RENDERBUFFER, _rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rboDepth);
    // Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        ZLOG_E("Framebuffer not complete!");
    _gBufferFBO.unbind();

    RF_GL_CHECK_ERROR();
}

void RfGeometryBufferGL::resize(const RfSize& fboSize)
{
    ZABORT_NOT_IMPLEMENTED();

    RF_GL_CHECK_ERROR();
}

void RfGeometryBufferGL::destroy()
{
    ZDELETEZ_SAFE(_gBuffer);
}

void RfGeometryBufferGL::bind()
{
    _gBufferFBO.bind();
}

void RfGeometryBufferGL::unbind()
{
    _gBufferFBO.unbind();
}

