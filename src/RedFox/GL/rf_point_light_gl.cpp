/*
RfPointLight - Implementation
*/
#include "rf_point_light_gl.h"
#include "zpd.h"

#include <algorithm>

#include <GL/glew.h>

#include "rf_light_manager_gl.h"
#include "rf_state_gl.h"

using namespace zootopia;

RfPointLightGL::RfPointLightGL(
    const RfColor& color
,   const RfPoint3& position)
    :
    _constant(1.0f)
,   _threshold(5.0f)
,   _maxBrightness(1.0f)
{
    // RfLight value
    //_color = color;
    //_maxBrightness = std::max(std::max((color.r()), (color.g())), (color.b())) / 255.0f;

    // pre-calculation.
    _calculateRadius();
}


RfPointLightGL::~RfPointLightGL() {}


void RfPointLightGL::setPosition(const RfPoint3& position)
{
    _values.position.x = position.x;
    _values.position.y = position.y;
    _values.position.z = position.z;

    /*
    RfLightManagerGL::_pointLightBuffer->bind();
    const GLintptr offset =
        RfScalarTruncToInt(_values.color.w) * sizeof(PointLight);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(RfVector3), &_values.position);
    RfLightManagerGL::_pointLightBuffer->setIndex(2);
    RfLightManagerGL::_pointLightBuffer->unbind();

    RF_GL_CHECK_ERROR();
    */
}


void RfPointLightGL::setColor(const RfColor& color)
{
    _values.color.x = color.r() / 255.0f;
    _values.color.y = color.g() / 255.0f;
    _values.color.z = color.b() / 255.0f;

    _maxBrightness = std::max(std::max((_values.color.x), (_values.color.y)), (_values.color.z));

    _calculateRadius();

    /*
    RfLightManagerGL::_pointLightBuffer->bind();
    const GLintptr offset = 
        RfScalarTruncToInt(_values.color.w) * sizeof(PointLight) + sizeof(RfVector4) * 1;
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(PointLight) - 1, &_values);
    RfLightManagerGL::_pointLightBuffer->setIndex(2);
    RfLightManagerGL::_pointLightBuffer->unbind();

    RF_GL_CHECK_ERROR();
    */
}


void RfPointLightGL::setProperties(const RfScalar linear, const RfScalar quadratic)
{
    _values.linear = linear;
    _values.quadratic = quadratic;

    _calculateRadius();

    /*
    RfLightManagerGL::_pointLightBuffer->bind();
    const GLintptr offset =
        RfScalarTruncToInt(_values.color.w) * sizeof(PointLight) + sizeof(RfVector4) * 1;
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(PointLight) - 1, &_values);
    RfLightManagerGL::_pointLightBuffer->setIndex(2);
    RfLightManagerGL::_pointLightBuffer->unbind();

    RF_GL_CHECK_ERROR();
    */
}


void RfPointLightGL::_calculateRadius()
{
    _values.radius = (-_values.linear + static_cast<float>(std::sqrt(_values.linear * _values.linear - 4 * _values.quadratic * (_constant - (256.0 / _threshold) * _maxBrightness)))) / (2 * _values.quadratic);
}

