#pragma once

/*
RfGeometryBufferGL - Class
*/

#ifndef __RF_GEOMETRY_BUFFER_GL_H__
#define __RF_GEOMETRY_BUFFER_GL_H__

#include <GL/glew.h>

#include "rf_size.h"

namespace zootopia {

    class RfGeometryBufferGL {

    private:

        RfGeometryBufferGL();
        ~RfGeometryBufferGL();

    public:

        static RfGeometryBufferGL* getBuffer();

        void initialize(const RfSize& fboSize);
        void resize(const RfSize& fboSize);
        void destroy();

        void bind();
        void unbind();

        GLuint getId() const { return _gBufferFBO; }
        GLuint getPositionDepthOutputTextureID() const { return _gPositionDepth; }
        GLuint getNormalOutputTextureID() const { return _gNormal; }
        GLuint getAlbedoSpecOutputTextureID() const { return _gAlbedoSpec; }

    private:

        GLuint  _gBufferFBO;

        GLuint  _gPositionDepth;
        GLuint  _gNormal;
        GLuint  _gAlbedoSpec;

        GLuint  _rboDepth;

    };

}

#endif
