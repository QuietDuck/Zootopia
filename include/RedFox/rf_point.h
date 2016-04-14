#pragma once

/*
RfPoint - Type
*/

#ifndef __RF_POINT_H__
#define __RF_POINT_H__

#include "rf_scalar.h"
#include "zpd.h"

namespace zootopia {

    class RfPoint {

    public:

        RfPoint() : x(0.0f), y(0.0f) {};

        RfPoint(RfScalar x, RfScalar y) {
            this->x = x;
            this->y = y;
        };

        RfPoint(const RfPoint &rhs) {
            x = rhs.x;
            y = rhs.y;
        };


        inline RfPoint & operator = (const RfPoint & rhs) {

            x = rhs.x;
            y = rhs.y;

            return *this;
        };

        inline const bool operator == (const RfPoint &rhs) const {

            return ((x EQ rhs.x) AND (y EQ rhs.y));
        };

        inline const bool operator != (const RfPoint &rhs) const {

            return ((x NEQ rhs.x) OR (y NEQ rhs.y));
        };

    public:

        RfScalar x;
        RfScalar y;

    };

    typedef RfPoint RfVector;

}

#endif