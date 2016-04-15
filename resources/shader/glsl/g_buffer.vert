#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 Tangent;
out vec3 BiTangent;

uniform mat4 MVP;
uniform mat4 M;

void main()
{
    vec4 _fragPos = M * position;
    
    //FragPos = position.xyz;
    FragPos = _fragPos.xyz;
    gl_Position = MVP * position;
    TexCoords = texCoords;

    mat3 normalMatrix = transpose(inverse(mat3(M)));
    Normal = normalMatrix * normal;
}