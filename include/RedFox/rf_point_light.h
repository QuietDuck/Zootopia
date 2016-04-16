#pragma once

/*
RfPointLight - Class
*/

#ifndef __RF_POINT_LIGHT_H__
#define __RF_POINT_LIGHT_H__

#include "rf_light.h"
#include "rf_scalar.h"

namespace zootopia {

    class RfPointLight : public RfLight {

    public:

        RfPointLight();
        ~RfPointLight();

    public:



    private:

        RfScalar    _constant;
        RfScalar    _linear;
        RfScalar    _quadratic;

    };

}

#endif