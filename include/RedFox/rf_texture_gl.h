#pragma once

#ifndef __RF_TEXTURE_GL_H__
#define __RF_TEXTURE_GL_H__

#include <GL/glew.h>

#include "rf_texture.h"

namespace zootopia {

    class RfTextureGL : public RfTexture {

    public:

        RfTextureGL();
        ~RfTextureGL();

    public:

        

    private:

        GLenum  _target;
        GLuint  _id;

    };

}

#endif