#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 Tangent;
in vec3 BiTangent;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

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
    
    /*
    vec3 _normal = texture(texture_normal1, TexCoords).rgb;

    if (_normal.r == 0.0 || _normal.g == 0.0 || _normal.b == 0)
        _normal = normalize(Normal);
    else
        _normal = normalize(_normal * 2.0 - 1.0);
    */
    gNormal = normalize(Normal);
    //gNormal = normalize(Normal * 2.0 - 1.0);
    // And the diffuse per-fragment color
    //gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    //gAlbedoSpec.rgb = material.diffuse;
    
    vec3 _albedo = texture(texture_diffuse1, TexCoords).rgb; 
    if (_albedo.r == 0.0 || _albedo.g == 0.0 || _albedo.b == 0)
        _albedo = material.diffuse;
    
    gAlbedoSpec.rgb = _albedo;

    // Store specular intensity in gAlbedoSpec's alpha component
    //gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
    float _alpha = texture(texture_specular1, TexCoords).r;
    if (_alpha == 0.0)
        _alpha = material.specular.r;
    
    gAlbedoSpec.a = _alpha;
}