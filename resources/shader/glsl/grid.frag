#version 450 core

in vec4 axis;

out vec4 fragColor;

void main()
{
	if (axis.x == 0.0)
	{
		fragColor = vec4(1.0, 0.0, 0.0, 1.0);	// Red
	}
	else
	if (axis.z == 0.0)
	{
		fragColor = vec4(0.0, 0.0, 1.0, 1.0);	// Green
	}
	else
	{
		fragColor = vec4(1.0, 1.0, 1.0, 1.0);	// White
	}
	
}                      