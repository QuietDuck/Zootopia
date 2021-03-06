#version 450 core
layout (location = 0) out vec4 gPositionDepth;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords; 
    vec3 Normal;
    mat3 TBN;
} fs_in;

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

const float NEAR = 0.1; // Projection matrix's near plane distance
const float FAR = 50.0f; // Projection matrix's far plane distance
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));	
}


void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    gPositionDepth.xyz = fs_in.FragPos;
    gPositionDepth.a = 50.0 - LinearizeDepth(gl_FragCoord.z);
    
    // Also store the per-fragment normals into the gbuffer
    vec3 _normal = texture(texture_normal1, fs_in.TexCoords).rgb;
    //_normal.z = -_normal.z;
    if (_normal.x == 0.0 && _normal.y == 0.0 && _normal.z == 0.0)
        _normal = normalize(fs_in.Normal);
    else {
         _normal = normalize(_normal * 2.0 - 1.0) * fs_in.TBN;
        //_normal = normalize(_normal * 2.0 - 1.0);
        //_normal = normalize(_normal) * fs_in.TBN;
        _normal.z = -_normal.z;
    }
       
    gNormal = _normal;
    
    // And the diffuse per-fragment color
    vec3 _albedo = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    if (_albedo.r == 0.0 && _albedo.g == 0.0 && _albedo.b == 0.0)
        _albedo += material.diffuse;
    
    gAlbedoSpec.rgb = _albedo;
    //gAlbedoSpec.rgb = vec3(0.95);

    // Store specular intensity in gAlbedoSpec's alpha component
    float _alpha = texture(texture_specular1, fs_in.TexCoords).r;
    if (_alpha == 0.0)
        _alpha = material.specular.r;
    
    gAlbedoSpec.a = _alpha;
}