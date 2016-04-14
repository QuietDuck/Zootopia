#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct Material
{
	vec3	ambient;
	vec3	diffuse;
	vec3	specular;
	vec3	emission;
	float	shininess;
};

uniform Material material;

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // And the diffuse per-fragment color
    //gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    //gAlbedoSpec.rgb = material.diffuse;
    
    vec3 albedo = texture(texture_diffuse1, TexCoords).rgb;
    
    if (albedo.r == 0.0 || albedo.g == 0.0 || albedo.b == 0)
        albedo = material.diffuse;
    
    gAlbedoSpec.rgb = albedo;

    // Store specular intensity in gAlbedoSpec's alpha component
    //gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
    gAlbedoSpec.a = material.specular.r;
}