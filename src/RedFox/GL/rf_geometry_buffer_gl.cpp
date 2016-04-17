/*
RfGeometryBufferGL - Implementation
*/
#include "rf_geometry_buffer_gl.h"
#include "zpd.h"

using namespace zootopia;

// SINGLE-TONE CLASS
RfGeometryBufferGL* RfGeometryBufferGL::_gBuffer = nullptr;

RfGeometryBufferGL::RfGeometryBufferGL() :
    _gBufferFBO(GL_NONE)
{}

RfGeometryBufferGL::~RfGeometryBufferGL() {}

RfGeometryBufferGL* RfGeometryBufferGL::getBuffer()
{
    if (_gBuffer) {
        return _gBuffer;
    }      
    else {
        _gBuffer = new RfGeometryBufferGL;
        return _gBuffer;
    }
}

void RfGeometryBufferGL::initialize(const RfSize & fboSize)
{

}

void RfGeometryBufferGL::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _gBufferFBO);
}

void RfGeometryBufferGL::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

