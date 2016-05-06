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
#include "rf_dir_light_gl.h"
#include "rf_spot_light_gl.h"
#include "rf_object_gl.h"

#include "rf_shader_storage_buffer_gl.h"

namespace zootopia {

    class RfLightManagerGL : public RfLightManager {

        friend class RfDirLightGL;
        friend class RfPointLightGL;
        friend class RfSpotLightGL;
        
    private:

        RfLightManagerGL();
        ~RfLightManagerGL();

    public:

        static RfLightManagerGL* getInstance();
        void destroy();

        void drawLightBulb();

    private:

        void _insertLight(RfLight* light) override;
        void _deleteLight(RfLight* light);

        static RfLightManagerGL*    _lightManager;

        RfShaderStorageBufferGL*    _dirLightBuffer;
        RfShaderStorageBufferGL*    _pointLightBuffer;
        RfShaderStorageBufferGL*    _spotLightBuffer;

        // Light Bulb
        RfShaderGL*     _lightBulbShader;
        RfModelGL*      _lightBulbModel;
        RfObjectGL*     _lightBulb;

    };

}


#endif