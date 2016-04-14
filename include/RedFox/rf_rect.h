#pragma once

#ifndef __RF_RECT_H__
#define __RF_RECT_H__

#include "rf_scalar.h"
#include "rf_point.h"
#include "rf_size.h"

namespace zootopia {

    class RfRect {

    public:

        RfRect() : origin(0.0f, 0.f), size(0.0f, 0.0f) {};

        RfRect(RfScalar x, RfScalar y, RfScalar w, RfScalar h) : origin(x, y), size(w, h) {};
        RfRect(RfScalar x, RfScalar y, const RfSize& size_) : origin(x, y), size(size_) {};
        RfRect(const RfPoint& origin_, RfScalar w, RfScalar h) : origin(origin_), size(w, h) {};

        RfRect(const RfRect& rhs) {
            origin = rhs.origin;
            size = rhs.size;
        };

        inline RfRect& operator = (const RfRect& rhs) {

            origin = rhs.origin;
            size = rhs.size;

            return *this;
        };

        inline const bool operator == (const RfRect& rhs) const {

            return (origin EQ rhs.origin AND size EQ rhs.size);
        };


        inline const bool operator != (const RfRect& rhs) const {

            return (origin NEQ rhs.origin OR size NEQ rhs.size);
        };


    public:

        RfPoint	origin;
        RfSize	size;

    };

}

#endif