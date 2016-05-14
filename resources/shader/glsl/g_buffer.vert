#version 450 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    mat3 TBN;
} vs_out;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;

void main()
{
    gl_Position = MVP * position;
    
    vec4 _fragPos = MV * position;
    vs_out.FragPos = _fragPos.xyz;
    vs_out.TexCoords = texcoords;

    // This is not good. we should precalculate normalMatrix.
    mat3 normalMatrix = transpose(inverse(mat3(MV)));
    vs_out.Normal = normalMatrix * normal;
    
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);
    vs_out.TBN = transpose(mat3(T, B, N));
}