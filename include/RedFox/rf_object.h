#pragma once

/*
RfObject - Interface
*/

#ifndef __RF_OBJECT_H__
#define __RF_OBJECT_H__

#include "rf_scalar.h"
#include "rf_point2.h"
#include "rf_point3.h"
#include "rf_texture.h"
#include "rf_mesh.h"
#include "rf_shader.h"
#include "rf_model.h"

namespace zootopia {

    class RfObject {

    public:

        RfObject() {};
        virtual ~RfObject() {};

    public:

        RfPoint3 getPosition() const { return _position; }
        RfScalar getRotateAngle() const { return _rotateAngle; }
        RfPoint3 getScaleFactor() const { return _scaleFactor; }
        RfScalar getOpacity() const { return _opacity; }

        virtual void setPosition(const RfPoint3& position) = 0;
        virtual void setRotateAngle(const RfScalar angle) = 0;
        virtual void setScaleFactor(const RfPoint3& factor) = 0;
        virtual void setOpacity(const RfScalar opacity) = 0;

        virtual void draw() = 0;

        virtual void translate(const RfPoint3& vector) = 0;
        virtual void rotate(const RfScalar angle, const RfPoint3& vector) = 0;
        virtual void scale(const RfPoint3& factor) = 0;

        /* event
        > OnClick
        */

    protected:

        RfPoint3    _position;
        RfScalar    _rotateAngle;
        RfPoint3    _scaleFactor;
        RfScalar    _opacity;

        // unique id -> color picking id?
        
        // pickable? clickable? hittested?
        // other property...?

    };

}

#endif
