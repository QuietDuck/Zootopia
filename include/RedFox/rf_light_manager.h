#pragma once

/*
RfLightManager - Interface
*/

#ifndef __RF_LIGHT_MANAGER_H__
#define __RF_LIGHT_MANAGER_H__

#include <vector>

#include "rf_light.h"
#include "rf_directional_light.h"
#include "rf_point_light.h"
#include "rf_spot_light.h"

#include "rf_noncopyable.h"

namespace zootopia {

    class RfLightManager : private RfNoncopyable {

    public:

        RfLightManager() {};
        ~RfLightManager() {};

    public:

        virtual void setLight(RfLight* light) = 0;

    protected:

        std::vector<RfDirectionalLight*>   _directionalLights;
        std::vector<RfPointLight*>         _pointLights;
        std::vector<RfSpotLight*>          _spotLights;

    };

}

#endif