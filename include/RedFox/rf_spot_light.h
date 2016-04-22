#pragma once

/*
RfSpotLight - Class
*/

#ifndef __RF_SPOT_LIGHT_H__
#define __RF_SPOT_LIGHT_H__

#include "rf_light.h"

#include "rf_scalar.h"
#include "rf_color.h"
#include "rf_point3.h"
#include "rf_point4.h"

#include "zpd.h"

namespace zootopia {

    class RfSpotLight : public RfLight {

        friend class RfLightManager;

    public:

        RfSpotLight() {}
        virtual ~RfSpotLight() {}

    public:

        virtual void setPosition(const RfPoint3& position) = 0;
        virtual void setDirection(const RfPoint3& position) = 0;
        virtual void setColor(const RfColor& color) = 0;

    protected:

        struct Data {

            RfPoint3 position;
            uint32 pad_1;

            RfVector3 direction;
            RfScalar cutOff;

            RfVector3 color;
            uint32 index;
        };

        Data _values;

    };

}

#endif