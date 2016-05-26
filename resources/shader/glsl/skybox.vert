#version 450 core
layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 VP;

void main()
{
    vec4 pos = VP * vec4(position, 1.0);
    gl_Position = pos.xyww;
    TexCoords = position;
}  