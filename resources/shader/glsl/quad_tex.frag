#version 450 core

in vec2 texcoord;
out vec4 fColor;

uniform sampler2D tex;

void main()
{
	fColor = texture(tex, texcoord);
}

