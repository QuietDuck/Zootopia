/*
RfSpotLightGL - Implementation
*/
#include "rf_spot_light_gl.h"
#include "zpd.h"

#include <glm/glm.hpp>

#include "rf_light_manager_gl.h"
#include "rf_state_gl.h"

using namespace zootopia;

RfSpotLightGL::RfSpotLightGL(
    const RfPoint3& position
,   const RfVector3& direction
,   const RfColor& color)
{
    _values.position = position;
    _values.direction = direction;

    _values.color.x = color.r() / 255.0f;
    _values.color.y = color.g() / 255.0f;
    _values.color.z = color.b() / 255.0f;

    // http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
    _values.constant = 1.0f;
    _values.linear = 0.045f;
    _values.quadratic = 0.0075f;

    _values.padding = 0;
    
    /// TEMPORARY set default value.
    _values.cutOff = glm::cos(glm::radians(12.5f));
    _values.outerCutOff = glm::cos(glm::radians(15.5f));

    RfLightManagerGL* lightManager = RfLightManagerGL::getInstance();
    lightManager->_insertLight(this);
}


RfSpotLightGL::~RfSpotLightGL() {}


void RfSpotLightGL::setPosition(const RfPoint3& position)
{
    _values.position = position;

    _update(
        sizeof(RfSpotLight::Data) * _values.index,
        sizeof(RfPoint3),
        &_values.position
    );
}


void RfSpotLightGL::setDirection(const RfVector3& direction)
{
    _values.direction = direction;

    _update(
        sizeof(RfSpotLight::Data) * _values.index + sizeof(RfPoint3) + sizeof(RfScalar),
        sizeof(RfPoint3),
        &_values.direction
    );
}


void RfSpotLightGL::setColor(const RfColor& color)
{
    _values.color.x = color.r() / 255.0f;
    _values.color.y = color.g() / 255.0f;
    _values.color.z = color.b() / 255.0f;

    _update(
        sizeof(RfSpotLight::Data) * _values.index + sizeof(RfPoint3) + sizeof(RfVector3) + sizeof(uint32) + sizeof(RfScalar) * 5,
        sizeof(RfVector3),
        &_values.color
    );
}


void RfSpotLightGL::setCutOff(const RfScalar cutOff, const RfScalar outerCutOff)
{
    _values.cutOff = glm::cos(glm::radians(cutOff));
    _values.outerCutOff = glm::cos(glm::radians(outerCutOff));

    _update(
        sizeof(RfSpotLight::Data) * _values.index + sizeof(RfPoint3) + sizeof(RfVector3) + sizeof(uint32) + sizeof(RfScalar),
        sizeof(RfScalar) * 2,
        &_values.cutOff
    );
}


void RfSpotLightGL::setProperties(const RfScalar linear, const RfScalar quadratic)
{
    _values.linear = linear;
    _values.quadratic = quadratic;

    _update(
        sizeof(RfSpotLight::Data) * _values.index + sizeof(RfPoint3) + sizeof(RfVector3) + sizeof(uint32) + sizeof(RfScalar) * 3,
        sizeof(RfScalar) * 2,
        &_values.linear
    );
}


void RfSpotLightGL::_update(const GLintptr offset, const GLsizeiptr size, const GLvoid* data)
{
    RfLightManagerGL* lightManager = RfLightManagerGL::getInstance();
    lightManager->_spotLightBuffer->bind();
    lightManager->_spotLightBuffer->uploadSubData(offset, size, data);
    lightManager->_spotLightBuffer->unbind();
}
