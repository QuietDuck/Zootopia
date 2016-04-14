#pragma once

#ifndef __RF_POINT3_H__
#define __RF_POINT3_H__

#include "rf_scalar.h"
#include "zpd.h" // It should be moved to impl.

namespace zootopia {

    class RfPoint3 {

    public:

        RfPoint3() : x(0.0f), y(0.0f) {};

        RfPoint3(RfScalar x, RfScalar y, RfScalar z) {
            this->x = x;
            this->y = y;
            this->z = z;
        };

        RfPoint3(const RfPoint3 &rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
        };


        inline RfPoint3 & operator = (const RfPoint3 & rhs) {

            x = rhs.x;
            y = rhs.y;
            z = rhs.z;

            return *this;
        };

        inline const bool operator == (const RfPoint3 &rhs) const {

            return ((x EQ rhs.x) AND (y EQ rhs.y) AND (z EQ rhs.z));
        };

        inline const bool operator != (const RfPoint3 &rhs) const {

            return ((x NEQ rhs.x) OR (y NEQ rhs.y) OR (z NEQ rhs.z));
        };

    public:

        RfScalar x;
        RfScalar y;
        RfScalar z;

    };

    typedef RfPoint3 RfVector3;

}


#endif
