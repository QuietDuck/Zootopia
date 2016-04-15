#include "rf_camera_gl.h"
#include "zpd.h"

#include <iostream>

#include "rf_scalar.h"

using namespace zootopia;

RfCameraGL::RfCameraGL(
    glm::vec3 position,
    glm::vec3 worldUp,
    RfScalar yaw, 
    RfScalar pitch
    )
{
    _position = position;
    _worldUp = worldUp;
    _yaw = yaw;
    _pitch = pitch;
    updateVectors();

    _viewMatrix = glm::lookAt(_position, _position + _front, _up);
    _projMatrix = glm::perspective(_zoom, 1280.0f / 720.0f, 0.1f, 100.0f);
}

RfCameraGL::~RfCameraGL() {}

void RfCameraGL::processKeyboard(Movement direction, RfScalar deltaTime)
{
    RfScalar velocity = _movementSpeed * deltaTime;

    switch (direction) {

    case kForward:
        _position += _front * velocity;
        break;

    case kBackward:
        _position -= _front * velocity;
        break;

    case kLeft:
        _position -= _right * velocity;
        break;

    case kRight:
        _position += _right * velocity;
        break;
    }

    _viewMatrix = glm::lookAt(_position, _position + _front, _up);
}

void RfCameraGL::processMouseMovement(RfScalar xoffset, RfScalar yoffset, bool constrainPitch)
{
    xoffset *= _mouseSensitivity;
    yoffset *= _mouseSensitivity;

    _yaw += xoffset;
    _pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (_pitch > 89.0f)
            _pitch = 89.0f;
        if (_pitch < -89.0f)
            _pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    updateVectors();

    _viewMatrix = glm::lookAt(_position, _position + _front, _up);
}

void RfCameraGL::processMouseScroll(RfScalar yoffset)
{
    std::cout << _zoom << std::endl;

    if (_zoom >= 1.0f AND _zoom <= 45.0f)
        _zoom -= yoffset;
    if (_zoom <= 1.0f)
        _zoom = 1.0f;
    if (_zoom >= 45.0f)
        _zoom = 45.0f;

    _projMatrix = glm::perspective(glm::radians(_zoom), 1280.0f / 720.0f, 0.1f, 100.0f);
}

void RfCameraGL::updateVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    _right = glm::normalize(glm::cross(_front, _worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    _up = glm::normalize(glm::cross(_right, _front));
}
