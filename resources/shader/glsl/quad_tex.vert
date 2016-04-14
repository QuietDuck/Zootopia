#version 450 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 vTexCoord;

out vec2 texcoord;

void main()
{
	gl_Position = vPosition;
	texcoord = vTexCoord;
}
	
