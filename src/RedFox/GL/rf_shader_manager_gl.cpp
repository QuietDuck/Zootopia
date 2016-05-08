/*
RfShaderManagerGL - Implementation
*/
#include "rf_shader_manager_gl.h"
#include "zpd.h"

#include <GL/glew.h>

#include "rf_state_gl.h"

using namespace zootopia;

// Single Tone Class
static RfShaderManagerGL* _shaderManager = nullptr;
static RfShaderGL* _currentShader = nullptr;
static RfShaderGL* _defaultShader = nullptr;

RfShaderManagerGL::RfShaderManagerGL()
{
    //_defaultShader = new RfShaderGL();
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


void RfShaderManagerGL::useDefaultShader()
{

}



