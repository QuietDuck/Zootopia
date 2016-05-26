#pragma once

/*
RfFrameBufferGL - Class
*/

#ifndef __RF_FRAME_BUFFER_GL__
#define __RF_FRAME_BUFFER_GL__

#include <GL/glew.h>

#include "rf_size.h"

namespace zootopia {

    class RfFrameBufferGL {

    public:

        RfFrameBufferGL();
        ~RfFrameBufferGL();

    public:

        void bind();
        static void unbind();

        GLuint getId() const { return _frameBufferObject; }

    private:

        GLuint  _frameBufferObject;

    };

}

#endif