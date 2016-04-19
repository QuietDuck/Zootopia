/*
RfStateGL - Implementation
*/
#include "rf_state_gl.h"
#include "zpd.h"

using namespace zootopia;

GLenum RfStateGL::errorCheckGL(char * fileName, char * funcName, int lineNumb)
{
    GLenum err;

    while ((err = glGetError()) NEQ GL_NO_ERROR) {

#ifdef _DEBUG
        switch (err)
        {
        case GL_INVALID_ENUM:
            dbg::__log_to_warning_channel(fileName, lineNumb, funcName,
                "GL Error: GL_INVAILD_ENUM %d(0x%x)", err, err);
            break;

        case GL_INVALID_VALUE:
            dbg::__log_to_warning_channel(fileName, lineNumb, funcName,
                "GL Error: GL_INVALID_VALUE %d(0x%x)", err, err);
            break;

        case GL_INVALID_OPERATION:
            dbg::__log_to_warning_channel(fileName, lineNumb, funcName,
                "GL Error: GL_INVALID_OPERATION %d(0x%x)", err, err);
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            dbg::__log_to_warning_channel(fileName, lineNumb, funcName,
                "GL Error: GL_INVALID_FRAMEBUFFER_OPERATION %d(0x%x)", err, err);
            break;

        case GL_OUT_OF_MEMORY:
            dbg::__log_to_warning_channel(fileName, lineNumb, funcName,
                "GL Error: GL_OUT_OF_MEMORY %d(0x%x)", err, err);
            break;

        default:
            dbg::__log_to_warning_channel(fileName, lineNumb, funcName,
                "GL Error=%d(0x%x)", err, err);
            break;
        }

#endif

        return err;

    }

    return err;
}