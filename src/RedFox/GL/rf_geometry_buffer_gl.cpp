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

