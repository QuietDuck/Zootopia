#pragma once

/*
RfLightManagerGL - Class
*/

#ifndef __RF_LIGHT_MANAGER_GL_H__
#define __RF_LIGHT_MANAGER_GL_H__

#include <vector>

#include "rf_light.h"
#include "rf_light_manager.h"

#include "rf_point_light_gl.h"
//#include "rf_directional_light_gl.h"
//#include "rf_spot_light_gl.h"

#include "rf_shader_storage_buffer_gl.h"

namespace zootopia {

    class RfLightManagerGL : public RfLightManager {

        friend class RfPointLightGL;

    private:

        RfLightManagerGL();
        ~RfLightManagerGL();

    public:

        static RfLightManagerGL* getInstance();

        void destroy();

        void setLight(RfLight* light) override;

    private:

        static RfLightManagerGL*    _lightManager;

        RfShaderStorageBufferGL*    _directionalLightBuffer;
        RfShaderStorageBufferGL*    _pointLightBuffer;
        RfShaderStorageBufferGL*    _spotLightBuffer;

    };

}


#endif