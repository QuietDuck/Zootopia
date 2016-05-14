#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = position;
    TexCoords = texCoords;
}