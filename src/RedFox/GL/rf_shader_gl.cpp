/*
RfShaderGL - Implementation
*/
#include "rf_shader_gl.h"
#include "zpd.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "rf_state_gl.h"

#define USE_OLD_VERSION 0

using namespace zootopia;

#if USE_OLD_VERSION

///////////////////////////////////////////////////////
bool check_compile_status(GLuint handle)
{
    GLint	status;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint	len;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
        GLchar  *buf_log = new GLchar[len + 1];
        glGetShaderInfoLog(handle, len, NULL, buf_log);
        std::cerr << "Compilation failed: " << buf_log << std::endl;
        delete[] buf_log;
    }
    return (status == GL_TRUE);
}

GLuint compile_shader(GLenum type, const char* source)
{
    GLuint	handle = glCreateShader(type);
    if (!handle)
    {
        std::cerr << "Shader object creation failed." << std::endl;
        return handle;
    }
    glShaderSource(handle, 1, &source, NULL);
    glCompileShader(handle);
    if (!check_compile_status(handle))
    {
        std::cerr << "Shader compilation failed." << std::endl;
        return 0;
    }
    return handle;
}

bool check_link_status(GLuint handle)
{
    GLint	status;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint	len;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &len);
        GLchar	*buf_log = new GLchar[len + 1];
        glGetProgramInfoLog(handle, len, NULL, buf_log);
        std::cerr << "Linking failed: " << buf_log << std::endl;
        delete[] buf_log;
    }
    return (status == GL_TRUE);
}

GLuint build_program(const char* src_vert, const char* src_frag)
{
    GLuint	h_vert = compile_shader(GL_VERTEX_SHADER, src_vert);
    if (!h_vert)
    {
        std::cerr << "Error while compiling the vertex shader" << std::endl;
        return 0;
    }
    GLuint	h_frag = compile_shader(GL_FRAGMENT_SHADER, src_frag);
    if (!h_frag)
    {
        std::cerr << "Error wihle compiling the fragment shader" << std::endl;
        return 0;
    }

    GLuint h_prog = glCreateProgram();
    if (!h_prog)
    {
        std::cerr << "Program object creation failed." << std::endl;
        return h_prog;
    }
    glAttachShader(h_prog, h_vert);
    glAttachShader(h_prog, h_frag);
    glLinkProgram(h_prog);
    if (!check_link_status(h_prog))	return 0;

    return h_prog;
}

// original source:
// http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
//
std::string get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

RfShaderGL::RfShaderGL(
    const GLchar* vertexPath,
    const GLchar* fragmentPath,
    const GLchar* geometryPath) {

    std::string src_vert = get_file_contents(vertexPath);
    std::string src_frag = get_file_contents(fragmentPath);

    _shaderProgObj = build_program(src_vert.c_str(), src_frag.c_str());
}

RfShaderGL::~RfShaderGL() {}

void RfShaderGL::_checkCompileErrors(GLuint shader, std::string type)
{
    // NOT IMPLEMENTED
}

void RfShaderGL::use() {

    glUseProgram(_shaderProgObj);
}

void RfShaderGL::destroy() {

    glDeleteProgram(_shaderProgObj);
}

#else

RfShaderGL::RfShaderGL(
    const GLchar* vertexPath,
    const GLchar* fragmentPath,
    const GLchar* geometryPath)
{
    ZASSERT(vertexPath);
    ZASSERT(fragmentPath);

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if (geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    // 2. Compile shaders
    GLuint vertex, fragment;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    _checkCompileErrors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    _checkCompileErrors(fragment, "FRAGMENT");

    // If geometry shader is given, compile geometry shader
    GLuint geometry;
    if (geometryPath != nullptr)
    {
        const GLchar * gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        _checkCompileErrors(geometry, "GEOMETRY");
    }

    // Shader Program
    _shaderProgObj = glCreateProgram();
    glAttachShader(_shaderProgObj, vertex);
    glAttachShader(_shaderProgObj, fragment);
    if (geometryPath != nullptr)
        glAttachShader(_shaderProgObj, geometry);
    glLinkProgram(_shaderProgObj);
    _checkCompileErrors(_shaderProgObj, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr)
        glDeleteShader(geometry);

    RF_GL_CHECK_ERROR();
}

RfShaderGL::~RfShaderGL() {}

void RfShaderGL::_checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "| ERROR::::SHADER-COMPILATION-ERROR of type: " << type << "|\n" << infoLog << "\n|-------------------------------------------------------|" << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "| ERROR::::PROGRAM-LINKING-ERROR of type: " << type << "|\n" << infoLog << "\n|-------------------------------------------------------|" << std::endl;
        }
    }

    RF_GL_CHECK_ERROR();
}

void RfShaderGL::use() {

    glUseProgram(_shaderProgObj);

    RF_GL_CHECK_ERROR();
}

void RfShaderGL::destroy() {

    glDeleteProgram(_shaderProgObj);

    RF_GL_CHECK_ERROR();
}

#endif
