#pragma once

/*
RfPointLight - Class
*/

#ifndef __RF_POINT_LIGHT_H__
#define __RF_POINT_LIGHT_H__

#include "rf_light.h"
#include "rf_scalar.h"

namespace zootopia {

    class RfPointLight : public RfLight {

    public:

        RfPointLight(const RfColor& color, const RfPoint3& position);
        ~RfPointLight();

    public:

        void setPosition(const RfPoint3& position);
        void setProperties(const RfScalar constant, const RfScalar linear, const RfScalar quadratic);

        RfScalar getRadius() { return _radius; }
        Type getType() const override { return Type::kPoint; };

    private:

        void calculateRadius();

        RfPoint3    _position;

        RfScalar    _constant;
        RfScalar    _linear;
        RfScalar    _quadratic;

        RfScalar    _threshold;
        RfScalar    _maxBrightness;
        RfScalar    _radius;
    };

}

#endif