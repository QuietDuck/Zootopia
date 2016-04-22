#pragma once

/*
RfLight - Interface
*/

#ifndef __RF_LIGHT_H__
#define __RF_LIGHT_H__

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

        virtual Type getType() const = 0;

    };

}

#endif
