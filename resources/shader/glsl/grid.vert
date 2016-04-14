#version 450 core

layout(location = 0) in vec4 aPosition;

out vec4 axis;

uniform mat4 MVP;

void main()
{	
	axis = aPosition;
	gl_Position = MVP * aPosition;
}