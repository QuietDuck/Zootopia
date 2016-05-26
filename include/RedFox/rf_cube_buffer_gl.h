#pragma once

/*
RfCubeBufferGL - Class
*/

#ifndef __RF_CUBE_BUFFER_GL_H__
#define __RF_CUBE_BUFFER_GL_H__

#include <GL/glew.h>

namespace zootopia {

    class RfCubeBufferGL {

    private:

        // SINGLE-TONE CLASS
        RfCubeBufferGL();
        ~RfCubeBufferGL();

    public:

        static RfCubeBufferGL* getBuffer();
        void destroy();

        void bind();
        static void unbind();

    private:

        GLuint  _cubeVAO;
        GLuint  _cubeVBO;

    };

}

#endif