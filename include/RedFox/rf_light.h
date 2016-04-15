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

        enum Type {
            kPoint,
            kSpot,

        };

    public:

        RfLight() {};
        virtual ~RfLight() {};

    protected:

        RfColor     _color;
        RfPoint3    _position;

    };

}

#endif
