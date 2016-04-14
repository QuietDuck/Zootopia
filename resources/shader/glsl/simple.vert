#version 450 core

layout(location = 0) in vec4 aPosition;

uniform mat4 MVP;

void main()
{	
	gl_Position = MVP * aPosition;
}