#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 MVP;

void main()
{	
    TexCoords = texCoords;
	gl_Position = MVP * position;
}