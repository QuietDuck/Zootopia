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
        void setRange(const GLuint index, const GLintptr offset, const GLsizeiptr size);

        void uploadData(const GLsizeiptr size, const GLvoid* data);
        void uploadSubData(const GLintptr offset, const GLsizeiptr size, const GLvoid* data);

    private:

        GLuint  _SSBO;

    };

}

#endif