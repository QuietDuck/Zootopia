#pragma once

/*
RfDirectionalLight - Class
*/

#ifndef __RF_DIRECTIONAL_LIGHT_H__
#define __RF_DIRECTIONAL_LIGHT_H__

#include "rf_light.h"

#include "rf_scalar.h"
#include "rf_color.h"
#include "rf_point3.h"
#include "rf_point4.h"

#include "zpd.h"

namespace zootopia {

    class RfDirectionalLight : public RfLight {

        friend class RfLightManager;

    public:

        RfDirectionalLight() {}
        virtual ~RfDirectionalLight() {}

    public:

        virtual void setDirection(const RfPoint3& position) = 0;
        virtual void setColor(const RfColor& color) = 0;

        Type getType() const override { return Type::kDirectional; }
        void setIndex(uint32 index) { _values.index = index; }

    protected:

        struct Data {

            RfVector3 direction;
            uint32 pad_1;

            RfVector3 color;
            uint32 index;
        };

        Data _values;

    };

}

#endif