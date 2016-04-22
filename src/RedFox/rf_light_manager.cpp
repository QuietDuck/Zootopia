/*
RfLightManager - Implementation
*/
#include "rf_light_manager.h"
#include "zpd.h"

#include "rf_point_light_gl.h"

using namespace zootopia;

RfLightManager::RfLightManager() {}
RfLightManager::~RfLightManager() {}

void RfLightManager::setLights(const std::vector<RfLight*>& lights)
{
    for (auto light : lights) {

        switch (light->getType()) {

        case RfLight::Type::kDirectional: {

            RfDirectionalLight* directionalLight = static_cast<RfDirectionalLight*>(light);
            directionalLight->_values.index = _directionalLights.size(); // Set Index.
            _directionalLights.push_back(directionalLight->_values);
            break;
        }

        case RfLight::Type::kPoint: {

            RfPointLight* pointLight = static_cast<RfPointLight*>(light);
            pointLight->_values.index = _pointLights.size(); // Set Index.
            _pointLights.push_back(pointLight->_values);
            break;
        }

        case RfLight::Type::kSpot: {

            RfSpotLight* spotLight = static_cast<RfSpotLight*>(light);
            spotLight->_values.index = _pointLights.size(); // Set Index.
            _spotLights.push_back(spotLight->_values);
            break;
        }

        default:

            ZABORT("Unexpected Process: RfCompositorGL::setLights()");
        }
    }

    uploadData();
}
