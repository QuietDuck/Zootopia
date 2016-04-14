#include "rf_grid_gl.h"
#include "zpd.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rf_compositor_gl.h"

using namespace zootopia;

RfGridGL::RfGridGL() :
    _gridVAO(0)
,   _gridVBO(0)
,   _gridShader(nullptr) {

    _gridShader = new RfShaderGL("shader/glsl/grid.vert", "shader/glsl/grid.frag");

    // TODO: make custom size grid.
    // or glsl shader grid?

    // Set the grid vertex position
    for (int i = 0; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            _vertices[i][j].x = (j - 50.0f);
            _vertices[i][j].y = 0.0f;
            _vertices[i][j].z = (i - 50.0f);
        }
    }

    // GENERATE VAO, VBO.
    glGenVertexArrays(1, &_gridVAO);
    glBindVertexArray(_gridVAO);

    glGenBuffers(1, &_gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _gridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // FOR SAFE. (UNBIND BUFFER)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

RfGridGL::~RfGridGL() {}

void RfGridGL::destroy() {

    if (_gridShader)
        ZDELETEZ_SAFE(_gridShader);

    if (_gridVAO NEQ 0)
        glDeleteBuffers(1, &_gridVAO);

    if (_gridVBO NEQ 0)
        glDeleteBuffers(1, &_gridVBO);

}

void RfGridGL::draw() {

    ZASSERT(RfCompositorGL::_displayCamera);

    // BIND GRID BUFFER
    _gridShader->use();
    glBindVertexArray(_gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _gridVBO);

    // SET CAMERA
    glUniformMatrix4fv(glGetUniformLocation(_gridShader->getShaderProgObj(), "MVP"),
        1, GL_FALSE,
        glm::value_ptr(RfCompositorGL::_displayCamera->getMatrix()));

    // DRAW GRID
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    for (int i = 0; i < 101; i++)
        glDrawArrays(GL_LINE_STRIP, 101 * i, 101);

    for (int i = 0; i < 101; i++) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 101 * sizeof(Point), (void *)(i * sizeof(Point)));
        glDrawArrays(GL_LINE_STRIP, 0, 101);
    }

    // UNBIND BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}