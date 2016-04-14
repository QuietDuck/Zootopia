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

namespace zootopia {

    class RfCameraGL : public RfCamera {

    public:

        explicit RfCameraGL();
        ~RfCameraGL();

    public:

        void setPosition(const RfPoint3& position) override;
        void setLookAt(const RfPoint3& lookAt) override;
        void setHeadUp(const HeadUp headUp) override;
        void setPerspective(const RfScalar fovy, const RfScalar aspect, const RfScalar near, const RfScalar far) override;

        glm::mat4 getViewMatrix() const { return _viewMatrix; }
        glm::mat4 getProjMatrix() const { return _projMatrix; }
        glm::mat4 getMatrix() const { return _projMatrix * _viewMatrix; }

        // move(): translate.
        void translate(const RfVector3& translate) override;
        void rotate(const RfScalar angle, const RfVector3& vector) override;

    private:

        glm::vec3   _position;
        glm::vec3   _lookAt;
        glm::vec3   _headUp;

        glm::mat4   _viewMatrix;
        glm::mat4   _projMatrix;

    };

}

#endif
