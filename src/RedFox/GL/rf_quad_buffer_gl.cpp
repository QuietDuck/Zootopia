/*
RfQuadBufferGL - Implementation.
*/
#include "rf_quad_buffer_gl.h"
#include "zpd.h"

#include <GL/glew.h>

using namespace zootopia;

// SINGLE-TONE CLASS
RfQuadBufferGL* RfQuadBufferGL::_quadBuffer = nullptr;

RfQuadBufferGL::RfQuadBufferGL() :
    _quadVAO(GL_NONE)
,   _quadVBO(GL_NONE)
{
    GLfloat quadVertices[] = {
        // Positions        // Texture Coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    // Setup plane VAO
    glGenVertexArrays(1, &_quadVAO);
    glGenBuffers(1, &_quadVBO);
    glBindVertexArray(_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

RfQuadBufferGL::~RfQuadBufferGL() {}

void RfQuadBufferGL::initialize()
{

}

void RfQuadBufferGL::destroy()
{
    if (_quadVAO NEQ GL_NONE) {
        glDeleteVertexArrays(1, &_quadVAO);
    }

    if (_quadVBO NEQ GL_NONE) {
        glDeleteBuffers(1, &_quadVBO);
    }

    // DELETE ITSELF?
    if (_quadBuffer) {
        ZDELETEZ_SAFE(_quadBuffer);
    }
}

RfQuadBufferGL* RfQuadBufferGL::getBuffer()
{
    if (_quadBuffer) {
        return _quadBuffer;
    }
    else {
        _quadBuffer = new RfQuadBufferGL;
        return _quadBuffer;
    }
}
