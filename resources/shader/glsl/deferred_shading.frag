#version 450 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct PointLight {
    vec4 Position;
    
    float Linear;
    float Quadratic;
    float Radius;
    float padding;
    
    vec4 Color;
};

layout (std430, binding = 2) buffer PointLightBuffer
{ 
    PointLight lights[];
};

//const int NR_LIGHTS = 3;
//uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{             
    // Retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // Then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);

    //for(int i = 0; i < NR_LIGHTS; ++i)
    for (int i = 0; i < lights.length(); i++)
    {
        // Calculate distance between light source and current fragment
        float distance = length(lights[i].Position.xyz - FragPos);
        if(distance < lights[i].Radius)
        {
            // Diffuse
            vec3 lightDir = normalize(lights[i].Position.xyz - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color.rgb;
            // Specular
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = lights[i].Color.rgb * spec * Specular;
            // Attenuation
            float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }
    }
    
    FragColor = vec4(lighting, 1.0);
    
    /*
    vec4 testColor = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < colors.length(); i++)
    {
        testColor += colors[i];
    }
    FragColor = testColor;
    */
    
    //FragColor = vec4(FragPos, 1.0);
    //FragColor = vec4(Normal, 1.0);
    //FragColor = vec4(Diffuse, 1.0);
    //FragColor = vec4(vec3(Specular), 1.0);
    //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
