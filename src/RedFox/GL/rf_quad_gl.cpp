/*
RfQuadGL - Implementation
*/
#include "rf_quad_gl.h"
#include "zpd.h"

#include "rf_state_gl.h"
#include "rf_quad_buffer_gl.h"

using namespace zootopia;

RfQuadGL::RfQuadGL()
{
    _quadBuffer = RfQuadBufferGL::getBuffer();
}

RfQuadGL::~RfQuadGL()
{
    _quadBuffer->destroy(); // NOT SO GOOD...
}

void RfQuadGL::draw()
{
    // SET SIZE...

    _quadBuffer->bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    _quadBuffer->unbind();

    RF_GL_CHECK_ERROR();
}