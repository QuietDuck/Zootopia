#pragma once

/*
RfGeometryBufferGL - Class
*/

#ifndef __RF_GEOMETRY_BUFFER_GL_H__
#define __RF_GEOMETRY_BUFFER_GL_H__

#include <GL/glew.h>

namespace zootopia {

    class RfGeometryBufferGL {

    private:

        RfGeometryBufferGL();
        ~RfGeometryBufferGL();

    public:

        static RfGeometryBufferGL* getBuffer();

        void initialize();
        void destroy();

        GLuint getGBufferFBO() const { return _gBufferFBO; }

    private:

        static RfGeometryBufferGL* _gBuffer;

        GLuint  _gBufferFBO;

    };

}

#endif
