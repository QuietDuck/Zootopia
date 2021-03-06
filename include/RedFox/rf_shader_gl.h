#pragma once

/*
RfShaderGL - Class
*/

#ifndef __RF_SHADER_GL_H__
#define __RF_SHADER_GL_H__

#include <string>
#include <GL/glew.h>

#include "rf_shader.h"

namespace zootopia {

    class RfShaderGL : public RfShader {

    public:

        explicit RfShaderGL(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);
        //explicit RfShaderGL(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = nullptr);
        ~RfShaderGL();

    public:

        void use();

        GLuint getShaderProgObj() const { return _shaderProgObj; }

    private:

        void _checkCompileErrors(GLuint shader, std::string type);

        GLuint      _shaderProgObj;

    };

}

#endif