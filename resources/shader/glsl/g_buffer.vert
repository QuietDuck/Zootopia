#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 MVP;
uniform mat4 M;

void main()
{
    vec4 _fragPos = M * position;
    
    //FragPos = position.xyz;
    FragPos = _fragPos.xyz;
    gl_Position = MVP * position;
    TexCoords = texCoords;

    Normal = normal;
}