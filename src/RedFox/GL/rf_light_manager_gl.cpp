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

const GLuint D_LIGHT_MAX_NUM = 16;
const GLuint P_LIGHT_MAX_NUM = 256;
const GLuint S_LIGHT_MAX_NUM = 128;

// Single Tone
RfLightManagerGL* RfLightManagerGL::_lightManager = nullptr;

RfLightManagerGL::RfLightManagerGL() 
{
    _dirLightBuffer = new RfShaderStorageBufferGL;
    _pointLightBuffer = new RfShaderStorageBufferGL;
    _spotLightBuffer = new RfShaderStorageBufferGL;

    // Allocate SSBO
    _dirLightBuffer->bind();
    _pointLightBuffer->uploadData(
        sizeof(RfDirLight::Data) * D_LIGHT_MAX_NUM,
        nullptr);
    _dirLightBuffer->setIndex(D_LIGHT_BUFFER_INDEX);
    _dirLightBuffer->unbind();

    _pointLightBuffer->bind();
    _pointLightBuffer->uploadData(
        sizeof(RfPointLight::Data) * P_LIGHT_MAX_NUM,
        nullptr);
    _pointLightBuffer->setIndex(P_LIGHT_BUFFER_INDEX);
    _pointLightBuffer->unbind();

    _spotLightBuffer->bind();
    _spotLightBuffer->uploadData(
        sizeof(RfSpotLight::Data) * S_LIGHT_MAX_NUM,
        nullptr);
    _spotLightBuffer->setIndex(S_LIGHT_BUFFER_INDEX);
    _spotLightBuffer->unbind();
}


RfLightManagerGL::~RfLightManagerGL()
{
    delete _dirLightBuffer;
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

        RfDirLightGL* dirLight = static_cast<RfDirLightGL*>(light);
        dirLight->_setIndex(_dirLights.size());
        _dirLights.push_back(dirLight);

        _dirLightBuffer->bind();
        _dirLightBuffer->uploadSubData(
            sizeof(RfDirLight::Data) * dirLight->_getIndex(),
            sizeof(RfDirLight::Data),
            &dirLight->getData()
        );
        _dirLightBuffer->setRange(
            D_LIGHT_BUFFER_INDEX,
            0,
            sizeof(RfDirLight::Data) * _dirLights.size()
        );

        _dirLightBuffer->unbind();

        break;
    }

    case RfLight::Type::kPoint: {

        RfPointLightGL* pointLight = static_cast<RfPointLightGL*>(light);
        pointLight->_setIndex(_pointLights.size());
        _pointLights.push_back(pointLight);

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

        RfSpotLightGL* spotLight = static_cast<RfSpotLightGL*>(light);
        spotLight->_setIndex(_spotLights.size());
        _spotLights.push_back(spotLight);

        _spotLightBuffer->bind();
        _spotLightBuffer->uploadSubData(
            sizeof(RfSpotLight::Data) * spotLight->_getIndex(),
            sizeof(RfSpotLight::Data),
            &spotLight->getData()
        );
        _spotLightBuffer->setRange(
            S_LIGHT_BUFFER_INDEX,
            0,
            sizeof(RfSpotLight::Data) * _spotLights.size()
        );
        _spotLightBuffer->unbind();

        break;
    }

    default:

        ZABORT("Unexpected Process: setLights()");
    }
}



