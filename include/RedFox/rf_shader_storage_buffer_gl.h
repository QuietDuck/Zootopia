#pragma once

/*
RfShaderStorageBufferGL - Class
*/

#ifndef __RF_SHADER_STORAGE_BUFFER_GL_H__
#define __RF_SHADER_STORAGE_BUFFER_GL_H__

#include <GL/glew.h>

namespace zootopia {

    class RfShaderStorageBufferGL {

    public:

        RfShaderStorageBufferGL();
        ~RfShaderStorageBufferGL();

    public:

        void bind();
        void unbind();
        void setIndex(const GLuint index);

        void uploadData(GLsizeiptr size, const GLvoid* data);
        //void uploadSubData();

    private:

        GLuint  _SSBO;

    };

}

#endif