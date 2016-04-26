/*
RfShaderManagerGL - Implementation
*/
#include "rf_shader_manager_gl.h"
#include "zpd.h"

#include "rf_state_gl.h"

using namespace zootopia;

// Single Tone Class
RfShaderManagerGL* RfShaderManagerGL::_shaderManager = nullptr;

RfShaderManagerGL::RfShaderManagerGL()
{

}


RfShaderManagerGL::~RfShaderManagerGL()
{
    for (auto shader : _shaders) {
        delete shader;
    }
}


RfShaderManagerGL* RfShaderManagerGL::getInstance()
{
    if (_shaderManager) {
        return _shaderManager;
    }
    else {
        _shaderManager = new RfShaderManagerGL;
        return _shaderManager;
    }
}

void RfShaderManagerGL::destroy()
{
    delete _shaderManager;
}



