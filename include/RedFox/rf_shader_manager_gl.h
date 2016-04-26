#pragma once

/*
RfShaderManagerGL - Class
*/

#ifndef __RF_SHADER_MANAGER_GL_H__
#define __RF_SHADER_MANAGER_GL_H__

#include "rf_shader_manager.h"

#include "rf_shader_gl.h"

namespace zootopia {

    class RfShaderManagerGL : public RfShaderManager {

    private:

        RfShaderManagerGL();
        ~RfShaderManagerGL();

    public:

        static RfShaderManagerGL* getInstance();
        void destroy();

        RfShader* getCurrentShader();

    private:
        
        static RfShaderManagerGL*   _shaderManager;

        void _insertShader();
        void _setCurrentShader();

    };

}

#endif
