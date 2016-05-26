/*
RfFrameBufferGL - Implementation
*/
#include "rf_frame_buffer_gl.h"
#include "zpd.h"

#include "rf_state_gl.h"

using namespace zootopia;

RfFrameBufferGL::RfFrameBufferGL()
{
    glGenFramebuffers(1, &_frameBufferObject);

    RF_GL_CHECK_ERROR();
}

RfFrameBufferGL::~RfFrameBufferGL()
{
    glDeleteFramebuffers(1, &_frameBufferObject);

    RF_GL_CHECK_ERROR();
}

void RfFrameBufferGL::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);

    RF_GL_CHECK_ERROR();
}

void RfFrameBufferGL::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    RF_GL_CHECK_ERROR();
}
