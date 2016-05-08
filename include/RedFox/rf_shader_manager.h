#pragma once

/*
RfShaderManager - Interface
*/

#ifndef __RF_SHADER_MANAGER_H__
#define __RF_SHADER_MANAGER_H__

#include <vector>

#include "rf_shader.h"
#include "rf_noncopyable.h"

namespace zootopia {

    class RfShaderManager : private RfNoncopyable {

    public:

    protected:

        std::vector<RfShader*>      _shaders;

    };

}

#endif