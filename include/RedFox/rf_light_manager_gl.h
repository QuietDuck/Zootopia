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

    public:

        RfLightManagerGL();
        ~RfLightManagerGL();

    public:

        void uploadData() override;

    private:

        static RfShaderStorageBufferGL* _directionalLightBuffer;
        static RfShaderStorageBufferGL* _pointLightBuffer;
        static RfShaderStorageBufferGL* _spotLightBuffer;

    };

}


#endif