#pragma once

/*
RfCameraGL - Class

TODO:
- Consider w(quarternion)
- Redesign(Refactoring)
*/

#ifndef __RF_CAMERA_GL_H__
#define __RF_CAMERA_GL_H__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rf_camera.h"
#include "rf_scalar.h"

namespace zootopia {

    class RfCameraGL : public RfCamera {

    public:

        explicit RfCameraGL(
            glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            RfScalar yaw = -90.0f,
            RfScalar pitch = 0.0f
            );

        ~RfCameraGL();

    public:

        // Override
        void processKeyboard(Movement direction, RfScalar deltaTime) override;
        void processMouseMovement(RfScalar xoffset, RfScalar yoffset, bool constrainPitch = true) override;
        void processMouseScroll(RfScalar yoffset) override;

        glm::mat4 getMatrix() const { return _projMatrix * _viewMatrix; };
        glm::vec3 getPosition() const { return _position; }

    private:

        void updateVectors() override;

        // Camera Attributes
        glm::vec3   _position;
        glm::vec3   _front;
        glm::vec3   _up;
        glm::vec3   _right; // weird name...
        glm::vec3   _worldUp;

        // Camera Matrix
        glm::mat4   _viewMatrix;
        glm::mat4   _projMatrix;

    };

}

#endif
