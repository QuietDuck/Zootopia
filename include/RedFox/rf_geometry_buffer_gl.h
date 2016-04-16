#pragma once

/*
RfGeometryBufferGL - Class
*/

#ifndef __RF_GEOMETRY_BUFFER_GL_H__
#define __RF_GEOMETRY_BUFFER_GL_H__

#include <GL/glew.h>

#include "rf_size.h"

namespace zootopia {

    class RfGeometryBufferGL {

    private:

        RfGeometryBufferGL();
        ~RfGeometryBufferGL();

    public:

        static RfGeometryBufferGL* getBuffer();

        void initialize(const RfSize& fboSize);
        void resize(const RfSize& fboSize);
        void destroy();

        void bind();
        void unbind();

    private:

        static RfGeometryBufferGL* _gBuffer;

        GLuint  _gBufferFBO;

    };

}

#endif
