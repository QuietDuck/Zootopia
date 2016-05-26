#pragma once

/*
RfQuadBufferGL - Class
*/

#ifndef __RF_QUAD_BUFFER_GL_H__
#define __RF_QUAD_BUFFER_GL_H__

#include <GL/glew.h>

namespace zootopia {

    class RfQuadBufferGL {

    private:

        // SINGLE-TONE CLASS
        RfQuadBufferGL();
        ~RfQuadBufferGL();

    public:

        static RfQuadBufferGL* getBuffer();
        void destroy();

        void bind();
        void unbind();

    private:

        GLuint  _quadVAO;
        GLuint  _quadVBO;

    };

}

#endif