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

const GLuint MAX_LIGHT_NUM = 1024;

// Single Tone
RfLightManagerGL* RfLightManagerGL::_lightManager = nullptr;

RfLightManagerGL::RfLightManagerGL() 
{
    _directionalLightBuffer = new RfShaderStorageBufferGL;
    _pointLightBuffer = new RfShaderStorageBufferGL;
    _spotLightBuffer = new RfShaderStorageBufferGL;

    // Allocate SSBO
    //_directionalLightBuffer->bind();

    //_directionalLightBuffer->setIndex(D_LIGHT_BUFFER_INDEX);
    //_directionalLightBuffer->unbind();

    _pointLightBuffer->bind();
    _pointLightBuffer->uploadData(
        sizeof(RfPointLight::Data) * MAX_LIGHT_NUM,
        nullptr);
    _pointLightBuffer->setIndex(P_LIGHT_BUFFER_INDEX);
    _pointLightBuffer->unbind();

    //_spotLightBuffer->bind();

    //_spotLightBuffer->setIndex(S_LIGHT_BUFFER_INDEX);
    //_spotLightBuffer->unbind();
}


RfLightManagerGL::~RfLightManagerGL()
{
    delete _directionalLightBuffer;
    delete _pointLightBuffer;
    delete _spotLightBuffer;

    delete _lightManager;
}

RfLightManagerGL * RfLightManagerGL::getInstance()
{
    if (_lightManager) {
        return _lightManager;
    }      
    else {
        _lightManager = new RfLightManagerGL;
        return _lightManager;
    }
}

void RfLightManagerGL::destroy()
{
    RfLightManagerGL::~RfLightManagerGL();
}


void RfLightManagerGL::setLight(RfLight * light)
{
    switch (light->getType()) {

    case RfLight::Type::kDirectional: {

        ZABORT_NOT_IMPLEMENTED();
        break;
    }

    case RfLight::Type::kPoint: {

        RfPointLightGL* pointLight = static_cast<RfPointLightGL*>(light);
        pointLight->_setIndex(_pointLights.size());
        _pointLights.push_back(pointLight);

        // Upload data
        _pointLightBuffer->bind();
        _pointLightBuffer->uploadSubData(
            sizeof(RfPointLight::Data) * pointLight->_getIndex(),
            sizeof(RfPointLight::Data),
            &pointLight->getData()
        );
        _pointLightBuffer->setRange(
            P_LIGHT_BUFFER_INDEX,
            0,
            sizeof(RfPointLight::Data) * _pointLights.size()
        );
        _pointLightBuffer->unbind();

        break;
    }

    case RfLight::Type::kSpot: {

        ZABORT_NOT_IMPLEMENTED();
        break;
    }

    default:

        ZABORT("Unexpected Process: setLights()");
    }
}



