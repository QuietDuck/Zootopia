#pragma once

/*
RfTextureGL - Class
*/

#ifndef __RF_TEXTURE_GL_H__
#define __RF_TEXTURE_GL_H__

#include <GL/glew.h>

#include "rf_texture.h"
#include "rf_size.h"

namespace zootopia {

    class RfTextureGL : public RfTexture {

    public:

        explicit RfTextureGL(
            const RfSize& size,
            const GLenum format,
            const GLenum type,
            const GLenum pixelFormat,
            const void* pixels,
            bool useMipmap = false);

        ~RfTextureGL();

    public:

        void bind();
        void unbind();

    private:

        GLuint  _id;

    };

}

#endif