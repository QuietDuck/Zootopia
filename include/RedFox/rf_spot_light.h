#pragma once

/*
RfSpotLight - Class
*/

#ifndef __RF_SPOT_LIGHT_H__
#define __RF_SPOT_LIGHT_H__

#include "rf_light.h"
#include "rf_scalar.h"
#include "rf_point3.h"

namespace zootopia {

    class RfSpotLight : public RfLight {

    public:

        RfSpotLight();
        ~RfSpotLight();

    public:



    private:

        RfPoint3    _position;

        RfVector3   _direction;
        RfScalar    _cutOff;

    };

}

#endif