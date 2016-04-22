#pragma once

/*
RfLightManager - Class
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

        RfLightManager();
        ~RfLightManager();

    public:

        void setLights(const std::vector<RfLight*>& lights);

        virtual void uploadData() = 0;

    protected:

        std::vector<RfDirectionalLight::Data>   _directionalLights;
        std::vector<RfPointLight::Data>         _pointLights;
        std::vector<RfSpotLight::Data>          _spotLights;

    };

}

#endif