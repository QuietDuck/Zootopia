/*
RfDirLightGL - Implementation
*/
#include "rf_dir_light_gl.h"
#include "zpd.h"

#include "rf_light_manager_gl.h"
#include "rf_state_gl.h"

using namespace zootopia;

RfDirLightGL::RfDirLightGL(
    const RfVector3& direction,
    const RfColor& color)
{
    _values.direction = direction;

    _values.color.x = color.r() / 255.0f;
    _values.color.y = color.g() / 255.0f;
    _values.color.z = color.b() / 255.0f;

    _values.padding = 0;

    // Set Light
    RfLightManagerGL* lightManager = RfLightManagerGL::getInstance();
    lightManager->_insertLight(this);
}

RfDirLightGL::~RfDirLightGL()
{
    RfLightManagerGL* lightManager = RfLightManagerGL::getInstance();
    lightManager->_deleteLight(this);
}

void RfDirLightGL::setDirection(const RfVector3& direction)
{
    _values.direction = direction;

    _update(
        sizeof(RfDirLight::Data) * _values.index,
        sizeof(RfVector3),
        &_values.direction
    );
}

void RfDirLightGL::setColor(const RfColor& color)
{
    _values.color.x = color.r() / 255.0f;
    _values.color.y = color.g() / 255.0f;
    _values.color.z = color.b() / 255.0f;

    _update(
        sizeof(RfDirLight::Data) * _values.index + sizeof(RfVector3) + sizeof(uint32),
        sizeof(RfVector3),
        &_values.color
    );
}

void RfDirLightGL::_update(const GLintptr offset, const GLsizeiptr size, const GLvoid * data)
{
    RfLightManagerGL* lightManager = RfLightManagerGL::getInstance();
    lightManager->_dirLightBuffer->bind();
    lightManager->_dirLightBuffer->uploadSubData(offset, size, data);
    lightManager->_dirLightBuffer->unbind();
}
