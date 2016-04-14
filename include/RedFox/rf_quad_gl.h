#pragma once

/*
RfQuadGL - Class
*/

#ifndef __RF_QUAD_GL_H__
#define __RF_QUAD_GL_H__

#include <GL/glew.h>

#include "rf_quad.h"

namespace zootopia {

    class RfQuadGL : public RfQuad {

    public:

        RfQuadGL();
        ~RfQuadGL();

    public:

        void draw() override;

    private:

        GLuint      _VAO;
        GLuint      _VBO;

    };

}

#endif