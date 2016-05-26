#pragma once

/*
RfPostProcessBufferGL - Class
*/

#ifndef __RF_POST_PROCESS_BUFFER_GL__
#define __RF_POST_PROCESS_BUFFER_GL__

#include "rf_size.h"
#include "rf_frame_buffer_gl.h"

namespace zootopia {

    class RfPostProcessBufferGL {

    public:

        RfPostProcessBufferGL();
        ~RfPostProcessBufferGL();

    public:

        static RfPostProcessBufferGL* getBuffer();

        void initialize(const RfSize& fboSize);
        void resize(const RfSize& fboSize);
        void destroy();

        void bind();
        void unbind();

        GLuint getOutputTexture() const { return _output; }
        
    private:

        RfFrameBufferGL _pBufferFBO;

        GLuint  _output;
        GLuint  _rboDepth;

    };

}

#endif
