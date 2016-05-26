#pragma once

/*
RfSkyBoxGL - Class
*/

#ifndef __RF_SKYBOX_GL_H__
#define __RF_SKYBOX_GL_H__

#include <GL/glew.h>

#include "rf_shader_gl.h"
#include "rf_cube_buffer_gl.h"

namespace zootopia {

    class RfSkyboxGL {

    public:

        RfSkyboxGL();
        ~RfSkyboxGL();

    public:

        void draw();

    private:

        RfShaderGL*     _skyboxShader;
        RfCubeBufferGL* _cubeBuffer;

        GLuint _skyboxTexture;

    };

}

#endif
