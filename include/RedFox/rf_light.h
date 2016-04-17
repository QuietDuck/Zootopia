#pragma once

/*
RfLight - Interface
*/

#ifndef __RF_LIGHT_H__
#define __RF_LIGHT_H__

#include "rf_color.h"
#include "rf_point3.h"

namespace zootopia {

    class RfLight {

    public:

        RfLight() {};
        virtual ~RfLight() {};

    public:

        enum Type {
            kPoint,
            kSpot,
            kDirectional
        };

        void setColor(RfColor color) { 
            _color = color; 
        };

        RfColor getColor() const {
            return _color;
        }

        virtual Type getType() const = 0;

    protected:

        RfColor     _color;

    };

}

#endif
