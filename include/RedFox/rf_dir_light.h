#pragma once

/*
RfDirLight - Interface
*/

#ifndef __RF_DIR_LIGHT_H__
#define __RF_DIR_LIGHT_H__

#include "rf_light.h"

#include "rf_scalar.h"
#include "rf_color.h"
#include "rf_point3.h"
#include "rf_point4.h"

#include "zpd.h"

namespace zootopia {

    class RfDirLight : public RfLight {

    public:

        RfDirLight() {}
        virtual ~RfDirLight() {}

    public:

        struct Data {

            RfVector3 direction; uint32 padding;

            RfVector3 color;
            uint32 index;
        };

        virtual void setDirection(const RfVector3& direction) = 0;
        virtual void setColor(const RfColor& color) = 0;

        Type getType() const override { return Type::kDirectional; }
        Data getData() const { return _values; }

    protected:

        Data _values;

    };

}

#endif