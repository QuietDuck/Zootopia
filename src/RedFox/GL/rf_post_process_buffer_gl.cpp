/*
RfPostProcessBufferGL - Implementation
*/
#include "rf_post_process_buffer_gl.h"
#include "zpd.h"

#include "rf_scalar.h"

#include "rf_state_gl.h"

using namespace zootopia;

// SINGLE-TONE CLASS
static RfPostProcessBufferGL* _pBuffer = nullptr;

RfPostProcessBufferGL::RfPostProcessBufferGL()
{
    glGenTextures(1, &_output);
    glGenRenderbuffers(1, &_rboDepth);

    RF_GL_CHECK_ERROR();
}

RfPostProcessBufferGL::~RfPostProcessBufferGL()
{
    glDeleteRenderbuffers(1, &_rboDepth);
    glDeleteTextures(1, &_output);

    RF_GL_CHECK_ERROR();
}

RfPostProcessBufferGL* RfPostProcessBufferGL::getBuffer()
{
    if (_pBuffer) {
        return _pBuffer;
    }
    else {
        _pBuffer = new RfPostProcessBufferGL;
        return _pBuffer;
    }
}

void RfPostProcessBufferGL::initialize(const RfSize & fboSize)
{
    const GLsizei width = RfScalarTruncToInt(fboSize.w);
    const GLsizei height = RfScalarTruncToInt(fboSize.h);

    _pBufferFBO.bind();
    // Output color buffer
    glBindTexture(GL_TEXTURE_2D, _output);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _output, 0);

    // Create and attach depth buffer (renderbuffer)
    glBindRenderbuffer(GL_RENDERBUFFER, _rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        ZLOG_E("Framebuffer not complete!");
    _pBufferFBO.unbind();

    RF_GL_CHECK_ERROR();
}

void RfPostProcessBufferGL::resize(const RfSize & fboSize)
{
    ZABORT_NOT_IMPLEMENTED();
}

void RfPostProcessBufferGL::destroy()
{
    ZDELETEZ_SAFE(_pBuffer);
}

void RfPostProcessBufferGL::bind()
{
    _pBufferFBO.bind();
}

void RfPostProcessBufferGL::unbind()
{
    _pBufferFBO.unbind();
}
