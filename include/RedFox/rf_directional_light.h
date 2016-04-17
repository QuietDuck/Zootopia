#pragma once

/*
RfDirectionalLight - Class
*/

#ifndef __RF_DIRECTIONAL_LIGHT_H__
#define __RF_DIRECTIONAL_LIGHT_H__

#include "rf_light.h"

namespace zootopia {

    class RfDirectionalLight : public RfLight {

    public:

        RfDirectionalLight();
        ~RfDirectionalLight();

    public:



    private:

        RfVector3   _direction;

    };

}

#endif