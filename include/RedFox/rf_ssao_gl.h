#pragma once

/*
RfSSAOGL - Class
*/

#ifndef __RF_SSAO_GL_H__
#define __RF_SSAO_GL_H__

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "rf_size.h"

#include "rf_shader_gl.h"
#include "rf_quad_buffer_gl.h"

namespace zootopia {

    class RfSSAOGL {

    private:

        RfSSAOGL();
        ~RfSSAOGL();

    public:

        static RfSSAOGL* getInstance();

        void initialize(const RfSize& fboSize);
        void resize(const RfSize& fboSize);
        void destroy();

        void draw();

        GLuint getOutputTextureID() const { return _ssaoBlurColorBuffer; }

    private:

        GLuint  _ssaoFBO;
        GLuint  _ssaoBlurFBO;

        GLuint  _ssaoColorBuffer;
        GLuint  _ssaoBlurColorBuffer;

        GLuint  _noiseTexture;

        RfShaderGL* _ssaoShader;
        RfShaderGL* _ssaoBlurShader;

        RfQuadBufferGL* _quadBuffer;

        std::vector<glm::vec3>  _ssaoKernel;

    };

}

#endif