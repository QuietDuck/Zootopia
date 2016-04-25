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

        struct Data {

            RfPoint3 position; RfScalar constant;
            RfVector3 direction; uint32 padding;
            
            RfScalar cutOff;
            RfScalar outerCutOff;
            RfScalar linear;
            RfScalar quadratic;
            
            RfVector3 color;
            uint32 index;
        };

        virtual void setPosition(const RfPoint3& position) = 0;
        virtual void setDirection(const RfVector3& direction) = 0;
        virtual void setColor(const RfColor& color) = 0;
        virtual void setCutOff(const RfScalar cutOff, const RfScalar outerCutOff) = 0;
        virtual void setProperties(const RfScalar linear, const RfScalar quadratic) = 0;

        Type getType() const { return Type::kSpot; }
        Data getData() const { return _values; }

    protected:

        Data _values;

    };

}

#endif