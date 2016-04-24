/*
RfShaderStorageBufferGL - Implementation
*/
#include "rf_shader_storage_buffer_gl.h"
#include "zpd.h"

#include <GL/glew.h>

#include "rf_state_gl.h"

using namespace zootopia;

RfShaderStorageBufferGL::RfShaderStorageBufferGL() :
    _SSBO(GL_NONE)
{
    glGenBuffers(1, &_SSBO);

    RF_GL_CHECK_ERROR();
}

RfShaderStorageBufferGL::~RfShaderStorageBufferGL() 
{
    if (_SSBO NEQ GL_NONE)
        glDeleteBuffers(1, &_SSBO);

    RF_GL_CHECK_ERROR();
}

void RfShaderStorageBufferGL::bind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);

    RF_GL_CHECK_ERROR();
}

void RfShaderStorageBufferGL::unbind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    RF_GL_CHECK_ERROR();
}

void RfShaderStorageBufferGL::setIndex(const GLuint index)
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, _SSBO);

    RF_GL_CHECK_ERROR();
}

void RfShaderStorageBufferGL::setRange(const GLuint index, const GLintptr offset, const GLsizeiptr size)
{
    //bind();
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, _SSBO, offset, size);
    //unbind();

    RF_GL_CHECK_ERROR();
}

void RfShaderStorageBufferGL::uploadData(const GLsizeiptr size, const GLvoid* data)
{
    //bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY);
    //unbind();

    RF_GL_CHECK_ERROR();
}

void RfShaderStorageBufferGL::uploadSubData(const GLintptr offset, const GLsizeiptr size, const GLvoid* data)
{
    //bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
    //unbind();

    RF_GL_CHECK_ERROR();
}

