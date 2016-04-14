#pragma once

#ifndef __RF_GRID_GL_H__
#define __RF_GRID_GL_H__

#include <GL/glew.h>

#include "rf_grid.h"
#include "rf_shader_gl.h"
#include "rf_camera_gl.h"

namespace zootopia {

    class RfGridGL : public RfGrid {

    public:

        RfGridGL();
        ~RfGridGL();

    public:

        void draw() override;
        void destroy() override;

    private:

        struct Point {
            GLfloat x;
            GLfloat y;
            GLfloat z;
        };

        Point       _vertices[101][101];

        GLuint      _gridVAO;
        GLuint      _gridVBO;

        RfShaderGL* _gridShader;

    };

}

#endif
