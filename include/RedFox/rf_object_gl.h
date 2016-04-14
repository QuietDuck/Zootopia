#pragma once

/*
RfObjectGL - Class

TODO:
- Redesign.
- Consider other platform(DX)
*/

#ifndef __RF_OBJECT_GL_H__
#define __RF_OBJECT_GL_H__

// STD LIBRARY
#include <vector>

// THIRD PARTY
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// INTERFACE
#include "rf_object.h"
#include "rf_shader.h"

// CLASS
#include "rf_model_gl.h"
#include "rf_texture_gl.h"
#include "rf_mesh_gl.h"

namespace zootopia {

    class RfObjectGL : public RfObject {

    public:

        RfObjectGL();
        RfObjectGL(RfModel* model);
        RfObjectGL(RfModelGL* model);

        ~RfObjectGL();

    public:

        void setPosition(const RfPoint3& position) override;
        void setRotateAngle(const RfScalar angle) override;
        void setScaleFactor(const RfPoint3& factor) override;
        void setOpacity(const RfScalar opacity) override;

        void draw() override;
        void destroy() override;

        void translate(const RfPoint3& vector) override;
        void rotate(const RfScalar angle, const RfPoint3& vector) override;
        void scale(const RfPoint3& factor) override;

    private:

        glm::mat4       _modelMatrix;

        RfModelGL*      _model;

    };

}

#endif