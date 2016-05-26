/*
RfSkyboxGL - Implementation
*/
#include "rf_skybox_gl.h"
#include "zpd.h"

#include <IL/il.h>

#include "rf_state_gl.h"
#include "rf_compositor_gl.h"

using namespace zootopia;

RfSkyboxGL::RfSkyboxGL()
{
    _skyboxShader = new RfShaderGL("shader/glsl/skybox.vert", "shader/glsl/skybox.frag");
    _skyboxShader->use();
    glUniform1i(glGetUniformLocation(_skyboxShader->getShaderProgObj(), "skybox"), 0);

    _cubeBuffer = RfCubeBufferGL::getBuffer();

    // Generate GL texture (skybox texture)
    glGenTextures(1, &_skyboxTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxTexture);

    // Generate DevIL image.
    ILuint	img;
    ilGenImages(1, &img);
    ilBindImage(img);

    ilLoadImage("textures/skybox/right.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
    ilLoadImage("textures/skybox/left.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
    ilLoadImage("textures/skybox/top.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
    ilLoadImage("textures/skybox/bottom.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
    ilLoadImage("textures/skybox/back.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
    ilLoadImage("textures/skybox/front.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, GL_NONE);

    // free DevIL image.
    ilDeleteImages(1, &img);

    RF_GL_CHECK_ERROR();
}


RfSkyboxGL::~RfSkyboxGL()
{
    // DO NOT DESTROY CUBE BUFFER.

    glDeleteTextures(1, &_skyboxTexture);
    ZDELETEZ_SAFE(_skyboxShader);

    RF_GL_CHECK_ERROR();
}


void RfSkyboxGL::draw()
{
    glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content

    _skyboxShader->use();

    glm::mat4 VP = 
        RfCompositorGL::_displayCamera->getProjMatrix() 
        * 
        // Remove any translation component of the view matrix
        glm::mat4(glm::mat3(RfCompositorGL::_displayCamera->getViewMatrix()));

    glUniformMatrix4fv(
        glGetUniformLocation(_skyboxShader->getShaderProgObj(), "VP"),
        1, GL_FALSE,
        glm::value_ptr(VP));

    _cubeBuffer->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    _cubeBuffer->unbind();

    glDepthFunc(GL_LESS); // Set depth function back to default

    RF_GL_CHECK_ERROR();
}
