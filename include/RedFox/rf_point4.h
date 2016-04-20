#pragma once

#ifndef __RF_POINT4_H__
#define __RF_POINT4_H__

#include "rf_scalar.h"
#include "zpd.h" // It should be moved to impl.

namespace zootopia {

    class RfPoint4 {

    public:

        RfPoint4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {};

        RfPoint4(RfScalar x, RfScalar y, RfScalar z, RfScalar w) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        };

        RfPoint4(const RfPoint4& rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;
        };


        inline RfPoint4 & operator = (const RfPoint4& rhs) {

            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;

            return *this;
        };

        inline const bool operator == (const RfPoint4& rhs) const {

            return ((x EQ rhs.x) AND (y EQ rhs.y) AND (z EQ rhs.z) AND (w EQ rhs.w));
        };

        inline const bool operator != (const RfPoint4& rhs) const {

            return ((x NEQ rhs.x) OR (y NEQ rhs.y) OR (z NEQ rhs.z) OR (w NEQ rhs.w));
        };

    public:

        RfScalar x;
        RfScalar y;
        RfScalar z;
        RfScalar w;

    };

    typedef RfPoint4 RfVector4;

}


#endif
