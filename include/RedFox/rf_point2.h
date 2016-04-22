#pragma once

/*
RfPoint - Type
*/

#ifndef __RF_POINT2_H__
#define __RF_POINT2_H__

#include "rf_scalar.h"
#include "zpd.h"

namespace zootopia {

    class RfPoint2 {

    public:

        // CONSTRUCTOR
        RfPoint2() : _x(0.0f), _y(0.0f) {};

        RfPoint2(RfScalar x, RfScalar y) {
            _x = x;
            _y = y;
        };

        RfPoint2(const RfPoint2& rhs) {
            _x = rhs._x;
            _y = rhs._y;
        };


        // METHOD
        void set(const RfPoint2& rhs) {
            _x = rhs._x;
            _y = rhs._y;
        }

        void set(const RfScalar x, const RfScalar y) {
            _x = x;
            _y = y;
        }

        void setX(const RfScalar x) {
            _x = x;
        }

        void setY(const RfScalar y) {
            _y = y;
        }

        RfScalar x() const { return _x; }
        RfScalar y() const { return _y; }

        //normalize()
        //length()
        //dot
        //cross
        //scale
        //translate

        // OPERATOR
        inline RfPoint2& operator = (const RfPoint2& rhs) {

            _x = rhs._x;
            _y = rhs._y;

            return *this;
        };

        inline const bool operator == (const RfPoint2& rhs) const {

            return ((_x EQ rhs._x) AND (_y EQ rhs._y));
        };

        inline const bool operator != (const RfPoint2& rhs) const {

            return ((_x NEQ rhs._x) OR (_y NEQ rhs._y));
        };

    private:

        RfScalar _x;
        RfScalar _y;

    };

    typedef RfPoint2 RfVector2;

}

#endif