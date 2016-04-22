/*
RfTextureGL - Implementaion
*/
#include "rf_texture_gl.h"
#include "zpd.h"

#include "rf_state_gl.h"

using namespace zootopia;

// It's 2D Texture Class.
RfTextureGL::RfTextureGL(
    const RfSize& size
,   const GLenum format
,   const GLenum type
,   const GLenum pixelFormat
,   const void* pixels
,   bool useMipmap)
    :
    _id(GL_NONE)
{
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Always call this? NO.
    glTexImage2D(GL_TEXTURE_2D, 0, format,
        RfScalarTruncToInt(size.w), RfScalarTruncToInt(size.h),
        0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    if (useMipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    RF_GL_CHECK_ERROR();
}


RfTextureGL::~RfTextureGL()
{
    if (_id NEQ GL_NONE)
        glDeleteTextures(1, &_id);

    // unbind?
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    RF_GL_CHECK_ERROR();
}


void RfTextureGL::bind()
{
    glBindTexture(GL_TEXTURE_2D, _id);

    RF_GL_CHECK_ERROR();
}


void RfTextureGL::unbind() // SHOULD BE A STATIC FUNCTION?
{
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    RF_GL_CHECK_ERROR();
}
