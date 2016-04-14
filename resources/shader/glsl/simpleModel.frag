#version 450 core

in vec2 TexCoords;
out vec4 fragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    fragColor.rgb = texture(texture_diffuse1, TexCoords).rgb;
    fragColor.a = texture(texture_specular1, TexCoords).r;
}                      