#pragma once

/*
RfCamera - Interface
*/

#ifndef __RF_CAMERA_H__
#define __RF_CAMERA_H__

#include "rf_scalar.h"

namespace zootopia {

    class RfCamera {

    public:

        RfCamera() :
            _movementSpeed(3.0f)
            , _mouseSensitivity(0.25f)
            , _zoom(45.0f) {};

        virtual ~RfCamera() {};

    public:

        enum Movement {
            kForward,
            kBackward,
            kLeft,
            kRight
        };

        virtual void processKeyboard(Movement direction, RfScalar deltaTime) = 0;
        virtual void processMouseMovement(RfScalar xoffset, RfScalar yoffset, bool constrainPitch = true) = 0;
        virtual void processMouseScroll(RfScalar yoffset) = 0;


    protected:

        virtual void updateVectors() = 0;

        // Eular Angles
        RfScalar    _yaw;
        RfScalar    _pitch;

        // Camera options
        RfScalar    _movementSpeed;
        RfScalar    _mouseSensitivity;
        RfScalar    _zoom;

    };

}

#endif