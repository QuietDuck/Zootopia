/*
RfLightManagerGL - Implementation
*/
#include "rf_light_manager_gl.h"
#include "zpd.h"

#include "rf_state_gl.h"
#include "rf_compositor_gl.h"

using namespace zootopia;

const GLuint D_LIGHT_BUFFER_INDEX = 1;
const GLuint P_LIGHT_BUFFER_INDEX = 2;
const GLuint S_LIGHT_BUFFER_INDEX = 3;

const GLuint D_LIGHT_MAX_NUM = 16;
const GLuint P_LIGHT_MAX_NUM = 256;
const GLuint S_LIGHT_MAX_NUM = 128;

// Single Tone
static RfLightManagerGL* _lightManager = nullptr;

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

    // light bulb. (for debugging)
    _lightBulbShader = new RfShaderGL("shader/glsl/light_bulb.vert", "shader/glsl/light_bulb.frag");
    _lightBulbModel = new RfModelGL("models/sphere/sphere.obj");
    _lightBulb = new RfObjectGL(_lightBulbModel);
    _lightBulb->scale(RfPoint3(0.1f, 0.1f, 0.1f));
}


RfLightManagerGL::~RfLightManagerGL()
{
    ZDELETEZ_SAFE(_lightBulbShader);
    ZDELETEZ_SAFE(_lightBulbModel);
    ZDELETEZ_SAFE(_lightBulb);

    ZDELETEZ_SAFE(_dirLightBuffer);
    ZDELETEZ_SAFE(_pointLightBuffer);
    ZDELETEZ_SAFE(_spotLightBuffer);
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
    ZDELETEZ_SAFE(_lightManager);
}


void RfLightManagerGL::drawLightBulb()
{
    _lightBulbShader->use();

    for (auto pointLight : _pointLights) {
        _lightBulb->setPosition(pointLight->getPosition());
        _lightBulb->scale(RfPoint3(0.2f, 0.2f, 0.2f));
        const RfVector3 lightColor = pointLight->getColor();
        glUniform3f(glGetUniformLocation(_lightBulbShader->getShaderProgObj(), "lightColor"),
            lightColor.x, lightColor.y, lightColor.z);
        _lightBulb->draw();
    }
}


void RfLightManagerGL::_insertLight(RfLight * light)
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

        ZABORT("Unexpected Process: _insertLights()");
    }
}


void RfLightManagerGL::_deleteLight(RfLight * light)
{
    switch (light->getType()) {

    case RfLight::Type::kDirectional: {

        RfDirLightGL* dirLight = static_cast<RfDirLightGL*>(light);

        std::vector<RfDirLight*>::const_iterator iter =
            _dirLights.begin() + dirLight->_getIndex();
        iter = _dirLights.erase(iter);

        _dirLightBuffer->bind();
        uint32 index = dirLight->_getIndex();
        for (; index < _dirLights.size(); ++index) {

            _dirLightBuffer->uploadSubData(
                sizeof(RfDirLight::Data) * index,
                sizeof(RfDirLight::Data),
                &_dirLights[index]->getData()
            );

            RfDirLightGL* temp = static_cast<RfDirLightGL*>(_dirLights[index]);
            temp->_setIndex(index);
        }

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

        // 1. delete vector element.
        std::vector<RfPointLight*>::const_iterator iter = 
            _pointLights.begin() + pointLight->_getIndex();
        iter = _pointLights.erase(iter);

        // 2. delete ssbo data. (append data into ssbo.)
        _pointLightBuffer->bind();
        uint32 index = pointLight->_getIndex();
        for ( ; index < _pointLights.size(); ++index) {
           
            _pointLightBuffer->uploadSubData(
                sizeof(RfPointLight::Data) * index,
                sizeof(RfPointLight::Data),
                &_pointLights[index]->getData()
            );

            RfPointLightGL* temp = static_cast<RfPointLightGL*>(_pointLights[index]);
            temp->_setIndex(index);
        }

        // 3. re-arrange ssbo data. (setRange and reset index.)
        // TODO: fix bug. there is a invalid value error if size is 0.
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

        std::vector<RfSpotLight*>::const_iterator iter =
            _spotLights.begin() + spotLight->_getIndex();
        iter = _spotLights.erase(iter);

        _spotLightBuffer->bind();
        uint32 index = spotLight->_getIndex();
        for (; index < _pointLights.size(); ++index) {

            _spotLightBuffer->uploadSubData(
                sizeof(RfSpotLight::Data) * index,
                sizeof(RfSpotLight::Data),
                &_spotLights[index]->getData()
            );

            RfSpotLightGL* temp = static_cast<RfSpotLightGL*>(_spotLights[index]);
            temp->_setIndex(index);
        }

        _spotLightBuffer->setRange(
            S_LIGHT_BUFFER_INDEX,
            0,
            sizeof(RfSpotLight::Data) * _spotLights.size()
        );
        _spotLightBuffer->unbind();

        break;
    }

    default:

        ZABORT("Unexpected Process: _deleteLights()");
    }

}



