/*
RfObjectGL - Implementation
*/
#include "rf_object_gl.h"
#include "zpd.h"

#include "rf_point3.h"
#include "rf_scalar.h"
#include "rf_point2.h"

#include "rf_state_gl.h"
#include "rf_compositor_gl.h"

using namespace zootopia;

RfObjectGL::RfObjectGL() :
    _modelMatrix(glm::mat4(1.0f))
,   _model(nullptr)
{

}

RfObjectGL::RfObjectGL(RfModel* model) :
    _modelMatrix(glm::mat4(1.0f))
,   _model(nullptr)
{
    ZASSERT(model);
    _model = static_cast<RfModelGL*>(model);
}

RfObjectGL::RfObjectGL(RfModelGL* model) :
    _modelMatrix(glm::mat4(1.0f))
,   _model(model)
{
    ZASSERT(model);
    _model = model; // for sure.
}

RfObjectGL::~RfObjectGL() {}

void RfObjectGL::setPosition(const RfPoint3& position)
{
    _position = position;
}

void RfObjectGL::setRotateAngle(const RfScalar angle)
{
    _rotateAngle = angle;
}

void RfObjectGL::setScaleFactor(const RfPoint3& factor)
{
    _scaleFactor = factor;
}

void RfObjectGL::setOpacity(const RfScalar opacity)
{
    _opacity = opacity;
}

void RfObjectGL::draw()
{
    glUniformMatrix4fv(
        glGetUniformLocation(RfCompositorGL::_deferredShader->getShaderProgObj(), "MVP"),
        1, GL_FALSE,
        glm::value_ptr(RfCompositorGL::_displayCamera->getMatrix() * _modelMatrix));

    glUniformMatrix4fv(
        glGetUniformLocation(RfCompositorGL::_deferredShader->getShaderProgObj(), "M"),
        1, GL_FALSE,
        glm::value_ptr(_modelMatrix));

    _model->draw();

    RF_GL_CHECK_ERROR();
}

void RfObjectGL::destroy()
{

}

void RfObjectGL::translate(const RfPoint3& vector)
{
    _modelMatrix = glm::translate(
        _modelMatrix,
        glm::vec3(vector.x, vector.y, vector.z)
        );
}

void RfObjectGL::rotate(const RfScalar angle, const RfPoint3& vector)
{
    // TODO: degree -> radian, radian -> degree
    _modelMatrix = glm::rotate(
        _modelMatrix,
        glm::radians(angle),
        glm::vec3(vector.x, vector.y, vector.z)
        );
}

void RfObjectGL::scale(const RfPoint3& factor)
{
    _modelMatrix = glm::scale(
        _modelMatrix,
        glm::vec3(factor.x, factor.y, factor.z)
        );
}



