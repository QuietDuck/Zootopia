#pragma once

#ifndef __RF_SIZE_H__
#define __RF_SIZE_H__

#include "rf_scalar.h"
#include "zpd.h"

namespace zootopia {

    class RfSize {

    public:

        RfSize() : w(0.f), h(0.f) {};

        RfSize(RfScalar w, RfScalar h) {
            this->w = w;
            this->h = h;
        };

        RfSize(const RfSize& rhs) {
            w = rhs.w;
            h = rhs.h;
        };


        inline RfSize& operator = (const RfSize& rhs) {

            w = rhs.w;
            h = rhs.h;

            return *this;
        };

        inline const bool operator == (const RfSize& rhs) const {

            return ((w EQ rhs.w) AND (h EQ rhs.h));
        };

        inline const bool operator != (const RfSize& rhs) const {

            return ((w NEQ rhs.w) AND (h NEQ rhs.h));
        };

    public:

        RfScalar w;
        RfScalar h;

    };

}

#endif
