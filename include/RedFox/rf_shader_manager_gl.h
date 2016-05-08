#pragma once

/*
RfShaderManagerGL - Class
*/

#ifndef __RF_SHADER_MANAGER_GL_H__
#define __RF_SHADER_MANAGER_GL_H__

#include <vector>

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
        
        void useDefaultShader();

    private:

        void _setCurrentShader();

    };

}

#endif
