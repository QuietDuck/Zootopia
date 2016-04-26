#pragma once

/*
RfLightManager - Interface
*/

#ifndef __RF_LIGHT_MANAGER_H__
#define __RF_LIGHT_MANAGER_H__

#include <vector>

#include "rf_light.h"
#include "rf_dir_light.h"
#include "rf_point_light.h"
#include "rf_spot_light.h"

#include "rf_noncopyable.h"

namespace zootopia {

    class RfLightManager : private RfNoncopyable {

    public:

        RfLightManager() {};
        ~RfLightManager() {};

    public:

    protected:

        virtual void _insertLight(RfLight* light) = 0;

        std::vector<RfDirLight*>    _dirLights;
        std::vector<RfPointLight*>  _pointLights;
        std::vector<RfSpotLight*>   _spotLights;

    };

}

#endif