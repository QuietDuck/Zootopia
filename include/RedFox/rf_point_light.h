#pragma once

/*
RfPointLight - Interface
*/

#ifndef __RF_POINT_LIGHT_H__
#define __RF_POINT_LIGHT_H__

#include "rf_light.h"

#include "rf_scalar.h"
#include "rf_color.h"
#include "rf_point3.h"
#include "rf_point4.h"

#include "zpd.h"

namespace zootopia {

    class RfPointLight : public RfLight {

    public:

        RfPointLight() {}
        virtual ~RfPointLight() {}

    public:

        struct Data {

            RfPoint3 position; RfScalar constant;

            uint32 padding;
            RfScalar linear;
            RfScalar quadratic;
            RfScalar radius;

            RfVector3 color;
            uint32 index;
        };

        virtual void setPosition(const RfPoint3& position) = 0;
        virtual void setColor(const RfColor& color) = 0;
        virtual void setProperties(const RfScalar linear, const RfScalar quadratic) = 0;

        RfPoint3    getPosition() const { return _values.position; }
        RfVector3   getColor() const { return _values.color; }

        Type getType() const override { return Type::kPoint; }
        Data getData() const { return _values; }

    protected:

        Data  _values;

    };

}

#endif