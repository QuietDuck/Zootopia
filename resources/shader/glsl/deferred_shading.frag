#version 450 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// Directional Light
struct DirLight {
    vec4 Direction;
    vec4 Color;
};
layout (std430, binding = 1) buffer DirLightBuffer
{ 
    DirLight dirLights[];
};

// Point Light
struct PointLight {
    vec4 Position;
    
    float padding;
    float Linear;
    float Quadratic;
    float Radius;
    
    vec4 Color;
};
layout (std430, binding = 2) buffer PointLightBuffer
{ 
    PointLight pointLights[];
};

// Spot Light
struct SpotLight {
    vec4 Position;
    vec4 Direction;
    
    float CutOff;
    float OuterCutOff;
    float Linear;
    float Quadratic;
    
    vec4 Color;
};
layout (std430, binding = 3) buffer SpotLightBuffer
{ 
    SpotLight spotLights[];
};


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
    
    // Calculate Directional Lights
    for (int i = 0; i < dirLights.length(); ++i) {
        
        // Diffuse
        vec3 lightDir = normalize(-dirLights[i].Direction.xyz);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * dirLights[i].Color.rgb; 
        // Specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = dirLights[i].Color.rgb * spec * Specular;
        lighting += diffuse + specular;
    }

    // Calculate Point Lights
    for (int i = 0; i < pointLights.length(); ++i) {
        
        // Calculate distance between light source and current fragment
        float distance = length(pointLights[i].Position.xyz - FragPos);
        if(distance < pointLights[i].Radius) {
            // Diffuse
            vec3 lightDir = normalize(pointLights[i].Position.xyz - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * pointLights[i].Color.rgb;
            // Specular
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = pointLights[i].Color.rgb * spec * Specular;
            // Attenuation
            //float attenuation = 1.0 / (pointLights[i].Position.w + pointLights[i].Linear * distance + pointLights[i].Quadratic * distance * distance);
            float attenuation = 1.0 / (1.0 + pointLights[i].Linear * distance + pointLights[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }
    }
    
    // Calculate Spot Lights
    //*
    for (int i = 0; i < spotLights.length(); ++i) {
        
        // Calculate distance between light source and current fragment
        float distance = length(spotLights[i].Position.xyz - FragPos);
        // Diffuse
        vec3 lightDir = normalize(spotLights[i].Position.xyz - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * spotLights[i].Color.rgb;
        // Specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = spotLights[i].Color.rgb * spec * Specular;
        // Spotlight
        float theta = dot(lightDir, normalize(-spotLights[i].Direction.xyz)); 
        float epsilon = (spotLights[i].CutOff - spotLights[i].OuterCutOff);
        float intensity = clamp((theta - spotLights[i].OuterCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;
        // Attenuation
        //float attenuation = 1.0 / (spotLights[i].Position.w + spotLights[i].Linear * distance + spotLights[i].Quadratic * (distance * distance));
        float attenuation = 1.0 / (1.0 + spotLights[i].Linear * distance + spotLights[i].Quadratic * (distance * distance));    
        diffuse  *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;
    }
    //*/
    
    FragColor = vec4(lighting, 1.0);
    //FragColor = vec4(FragPos, 1.0);
    //FragColor = vec4(Normal, 1.0);
    //FragColor = vec4(Diffuse, 1.0);
    //FragColor = vec4(vec3(Specular), 1.0);
}