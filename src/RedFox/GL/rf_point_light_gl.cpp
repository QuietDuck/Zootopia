/*
RfPointLight - Implementation
*/
#include "rf_point_light_gl.h"
#include "zpd.h"

#include <algorithm>

#include "rf_light_manager_gl.h"
#include "rf_state_gl.h"

using namespace zootopia;

RfPointLightGL::RfPointLightGL(
    const RfPoint3& position
,   const RfColor& color
)
    :
    _threshold(5.0f)
,   _maxBrightness(1.0f)
{
    _values.position = position;

    _values.color.x = color.r() / 255.0f;
    _values.color.y = color.g() / 255.0f;
    _values.color.z = color.b() / 255.0f;

    _maxBrightness = std::max(std::max((_values.color.x), (_values.color.y)), (_values.color.z));

    // http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
    _values.constant = 1.0f;
    _values.linear = 0.22f;
    _values.quadratic = 0.2f;

    _values.padding = 0;

    // Pre-calculation.
    _calculateRadius();

    // Set Light
    RfLightManagerGL* lightManager = RfLightManagerGL::getInstance();
    lightManager->_insertLight(this);

}


RfPointLightGL::~RfPointLightGL()
{
    RfLightManagerGL* lightManager = RfLightManagerGL::getInstance();
    lightManager->_deleteLight(this);
}


void RfPointLightGL::setPosition(const RfPoint3& position)
{
    _values.position = position;

    // update Position
    _update(
        sizeof(RfPointLight::Data) * _values.index,
        sizeof(RfPoint3),
        &_values.position
    );
}


void RfPointLightGL::setColor(const RfColor& color)
{
    _values.color.x = color.r() / 255.0f;
    _values.color.y = color.g() / 255.0f;
    _values.color.z = color.b() / 255.0f;

    _maxBrightness = std::max(std::max((_values.color.x), (_values.color.y)), (_values.color.z));

    _calculateRadius();

    // update Radius, Color
    _update(
        sizeof(RfPointLight::Data) * _values.index + sizeof(RfPoint3) + sizeof(RfScalar) * 3 + sizeof(uint32),
        sizeof(RfScalar) + sizeof(RfVector3),
        &_values.radius
    );
}


void RfPointLightGL::setProperties(const RfScalar linear, const RfScalar quadratic)
{
    _values.linear = linear;
    _values.quadratic = quadratic;

    _calculateRadius();

    // update Linear, Quadratic, Radius
    _update(
        sizeof(RfPointLight::Data) * _values.index + sizeof(RfPoint3) + sizeof(RfScalar) + sizeof(uint32),
        sizeof(RfScalar) * 3,
        &_values.linear
    );
}


void RfPointLightGL::_calculateRadius()
{
    _values.radius = (-_values.linear + static_cast<float>(std::sqrt(_values.linear * _values.linear - 4 * _values.quadratic * (_values.constant - (256.0 / _threshold) * _maxBrightness)))) / (2 * _values.quadratic);
}


void RfPointLightGL::_update(const GLintptr offset, const GLsizeiptr size, const GLvoid* data)
{
    RfLightManagerGL* lightManager = RfLightManagerGL::getInstance();
    lightManager->_pointLightBuffer->bind();
    lightManager->_pointLightBuffer->uploadSubData(offset, size, data);
    lightManager->_pointLightBuffer->unbind();
}

