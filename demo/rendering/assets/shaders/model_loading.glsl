#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    Normal = aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec3 FragPos; 
in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;

uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;

uniform int DiffuseNum;
uniform int SpecularNum;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;

uniform vec3 viewPos;

void main()
{    
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    // vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    vec3 diffuse_map = vec3(1.0, 1.0, 1.0);
    
    if(DiffuseNum > 3)
    {
        diffuse_map = diffuse_map + vec3(texture(texture_diffuse4, TexCoords));
    }
    if(DiffuseNum > 2)
    {
        diffuse_map = diffuse_map + vec3(texture(texture_diffuse3, TexCoords));
    }
    if(DiffuseNum > 1)
    {
        diffuse_map = diffuse_map + vec3(texture(texture_diffuse2, TexCoords));
    }
    if(DiffuseNum > 0)
    {
        diffuse_map = diffuse_map + vec3(texture(texture_diffuse1, TexCoords));
    }

    vec3 specular_map = vec3(1.0, 1.0, 1.0);
    if(SpecularNum > 3)
    {
        specular_map = specular_map + vec3(texture(texture_specular4, TexCoords));
    }
    if(SpecularNum > 2)
    {
        specular_map = specular_map + vec3(texture(texture_specular3, TexCoords));
    }
    if(SpecularNum > 1)
    {
        specular_map = specular_map + vec3(texture(texture_specular2, TexCoords));
    }
    if(SpecularNum > 0)
    {
        specular_map = specular_map + vec3(texture(texture_specular1, TexCoords));
    }

    // 环境光
    vec3 ambient = light.ambient * material.ambient;

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 diffuse = light.diffuse * (diff * diffuse_map);

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular * (spec * material.specular);  
    vec3 specular = light.specular * (spec * specular_map);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}