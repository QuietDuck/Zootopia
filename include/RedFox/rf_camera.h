#pragma once

/*
RfCamera - Interface
*/

#ifndef __RF_CAMERA_H__
#define __RF_CAMERA_H__

#include "rf_scalar.h"
#include "rf_point3.h"

namespace zootopia {

    class RfCamera {

    public:

        RfCamera() {};
        virtual ~RfCamera() {};

    public:

        enum HeadUp {
            kAxis_X,
            kAxis_Y,
            kAxis_Z
        };

        virtual void setPosition(const RfPoint3& position) = 0;
        virtual void setLookAt(const RfPoint3& lookAt) = 0;
        virtual void setHeadUp(const HeadUp headUp) = 0;
        virtual void setPerspective(const RfScalar fovy, const RfScalar aspect, const RfScalar near, const RfScalar far) = 0;

        virtual void translate(const RfVector3& translate) = 0;
        virtual void rotate(const RfScalar angle, const RfVector3& vector) = 0;

    protected:

    };

}

#endif