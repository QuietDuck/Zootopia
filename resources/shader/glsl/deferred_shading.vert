#version 450 core
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vPosition;
    TexCoords = vTexCoords;
}