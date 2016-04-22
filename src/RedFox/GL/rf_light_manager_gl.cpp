/*
RfLightManagerGL - Implementation
*/
#include "rf_light_manager_gl.h"
#include "zpd.h"

#include "rf_state_gl.h"

using namespace zootopia;

const GLuint D_LIGHT_BUFFER_INDEX = 1;
const GLuint P_LIGHT_BUFFER_INDEX = 2;
const GLuint S_LIGHT_BUFFER_INDEX = 3;

RfShaderStorageBufferGL* RfLightManagerGL::_directionalLightBuffer = nullptr;
RfShaderStorageBufferGL* RfLightManagerGL::_pointLightBuffer = nullptr;
RfShaderStorageBufferGL* RfLightManagerGL::_spotLightBuffer = nullptr;

// It should have single tone pattern?
RfLightManagerGL::RfLightManagerGL() 
{
    _directionalLightBuffer = new RfShaderStorageBufferGL;
    _pointLightBuffer = new RfShaderStorageBufferGL;
    _spotLightBuffer = new RfShaderStorageBufferGL;
}


RfLightManagerGL::~RfLightManagerGL()
{
    delete _directionalLightBuffer;
    delete _pointLightBuffer;
    delete _spotLightBuffer;
}

void RfLightManagerGL::uploadData()
{
    _directionalLightBuffer->bind();
    // upload
    _directionalLightBuffer->setIndex(D_LIGHT_BUFFER_INDEX);
    _directionalLightBuffer->unbind();

    _pointLightBuffer->bind();
    _pointLightBuffer->uploadData(
        sizeof(RfPointLight::Data) * _pointLights.size(),
        _pointLights.data());
    _pointLightBuffer->setIndex(P_LIGHT_BUFFER_INDEX);
    _pointLightBuffer->unbind();

    _spotLightBuffer->bind();
    // upload
    _spotLightBuffer->setIndex(S_LIGHT_BUFFER_INDEX);
    _spotLightBuffer->unbind();
}




