#version 450 core
layout (location = 0) in vec4 position;
//layout (location = 1) in vec3 normal;
//layout (location = 2) in vec2 texCoords;

uniform mat4 MVP;
uniform mat4 M;

void main()
{
    gl_Position = MVP * position;
}