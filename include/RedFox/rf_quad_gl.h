#pragma once

/*
RfQuadGL - Class
*/

#ifndef __RF_QUAD_GL_H__
#define __RF_QUAD_GL_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rf_quad.h"

#include "rf_quad_buffer_gl.h"

namespace zootopia {

    class RfQuadGL : public RfQuad {

    public:

        RfQuadGL();
        ~RfQuadGL();

    public:

        void draw() override;

    private:
        
        RfQuadBufferGL* _quadBuffer;
        
        glm::mat4       _modelMatrix;

    };

}

#endif