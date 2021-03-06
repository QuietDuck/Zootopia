#pragma once

/*
RfQuad - Interface
*/

#ifndef __RF_QUAD_H__
#define __RF_QUAD_H__

#include "rf_point2.h"
#include "rf_size.h"

namespace zootopia {

    class RfQuad {

    public:

        RfQuad() {};
        virtual ~RfQuad() {};

    public:

        virtual void draw() = 0;

    protected:

        RfPoint2    _position;
        RfSize      _size;

    };

}

#endif