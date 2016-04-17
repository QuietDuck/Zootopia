/*
RfTextureGL - Implementaion
*/
#include "rf_texture_gl.h"
#include "zpd.h"

using namespace zootopia;

RfTextureGL::RfTextureGL(
    const RfSize& size
,   const void* pixels
,   bool useMipmap)
    :
    _target(GL_TEXTURE_2D)
,   _id(0)
{
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Always call this? NO.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        RfScalarTruncToInt(size.w), RfScalarTruncToInt(size.h),
        0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    if (useMipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}


RfTextureGL::~RfTextureGL()
{
    if (_id NEQ GL_NONE)
        glDeleteTextures(1, &_id);

    // unbind?
    glBindTexture(_target, 0);
}


void RfTextureGL::bind()
{
    glBindTexture(_target, _id);
}


void RfTextureGL::unbind() // SHOULD BE A STATIC FUNCTION?
{
    glBindTexture(_target, 0);
}
