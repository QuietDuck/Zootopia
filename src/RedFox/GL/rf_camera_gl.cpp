#include "rf_camera_gl.h"
#include "zpd.h"

using namespace zootopia;

RfCameraGL::RfCameraGL() :
    _position(glm::vec3(0,0,0))
,   _lookAt(glm::vec3(0,0,0))
,   _headUp(glm::vec3(0,1,0))
{}
RfCameraGL::~RfCameraGL() {}


void RfCameraGL::setPosition(const RfPoint3& position)
{   
    _position = glm::vec3(position.x, position.y, position.z);

    _viewMatrix = glm::lookAt(
        _position,
        _lookAt,
        _headUp
        );
}

void RfCameraGL::setLookAt(const RfPoint3& lookAt)
{
    _lookAt = glm::vec3(lookAt.x, lookAt.y, lookAt.z);

    _viewMatrix = glm::lookAt(
        _position,
        _lookAt,
        _headUp
        );
}

void RfCameraGL::setHeadUp(HeadUp headUp)
{
    switch (headUp) {

    case HeadUp::kAxis_X:
        _headUp = glm::vec3(1, 0, 0);
        break;

    case HeadUp::kAxis_Y:
        _headUp = glm::vec3(0, 1, 0);
        break;

    case HeadUp::kAxis_Z:
        _headUp = glm::vec3(0, 0, 1);
        break;

    default: // Y Axis headUp
        _headUp = glm::vec3(0, 1, 0);
        break;
    }
    
    _viewMatrix = glm::lookAt(
        _position,
        _lookAt,
        _headUp
        );
}

void RfCameraGL::setPerspective(const RfScalar fovy, const RfScalar aspect, const RfScalar near, const RfScalar far)
{
    _projMatrix = glm::perspective(fovy, aspect, near, far);
}

void RfCameraGL::translate(const RfVector3& vector)
{
    _viewMatrix = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(vector.x, vector.y, vector.z)
        ) * _viewMatrix;
}

void RfCameraGL::rotate(const RfScalar angle, const RfVector3& vector)
{
    _viewMatrix = glm::rotate(
        glm::mat4(1.0f),
        glm::radians(angle),
        glm::vec3(vector.x, vector.y, vector.z)
        ) * _viewMatrix;
}