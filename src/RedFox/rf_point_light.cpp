/*
RfPointLight - Implementation
*/
#include "rf_point_light.h"
#include "zpd.h"

#include <algorithm>

using namespace zootopia;

RfPointLight::RfPointLight(
    const RfColor& color
,   const RfPoint3& position)
    :
    _position(position)
,   _constant(1.0)
,   _linear(0.7)
,   _quadratic(1.8)
,   _threshold(5.0)
,   _maxBrightness(1.0)
,   _radius(0)
{
    // RfLight value
    _color = color;
    _maxBrightness = std::max(std::max((color.r()), (color.g())), (color.b())) / 255.0f;

    calculateRadius();
}


RfPointLight::~RfPointLight() {}


void RfPointLight::setPosition(const RfPoint3& position)
{
    _position = position;
}


void RfPointLight::setProperties(const RfScalar constant, const RfScalar linear, const RfScalar quadratic)
{
    _constant = constant;
    _linear = linear;
    _quadratic = quadratic;

    calculateRadius();
}


void RfPointLight::calculateRadius()
{
    _radius = (-_linear + static_cast<float>(std::sqrt(_linear * _linear - 4 * _quadratic * (_constant - (256.0 / _threshold) * _maxBrightness)))) / (2 * _quadratic);
}

