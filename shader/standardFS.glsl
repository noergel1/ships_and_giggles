#version 430 core

in GS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
} fs_in;

layout (std140, binding = 2) uniform dirLight
{
    vec3 dirDirection;
    vec3 dirAmbient;
    vec3 dirDiffuse;
    vec3 dirSpecular;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;

    sampler2D emission;
}; 

uniform Material material1;

// camera position
layout (std140, binding = 4) uniform Camera
{
    vec3 cameraPosition;
};

// Light Calculation Functions
vec3 CalcDirLight();

vec3 norm = normalize(fs_in.Normal);
vec3 lightDir = normalize(-fs_in.FragPos);
vec3 viewDir = normalize(cameraPosition - fs_in.FragPos);
vec3 reflectDir = reflect(-lightDir, norm);

vec3 texColorAmbient = vec3(texture(material1.diffuse, fs_in.TexCoord));
vec3 texColorDiffuse = vec3(texture(material1.diffuse, fs_in.TexCoord));
vec3 texColorSpecular = vec3(texture(material1.specular, fs_in.TexCoord));


void main()
{
    vec3 result = vec3(0.0f);
    result += CalcDirLight();

    gl_FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight()
{
    vec3 lightDir = normalize(-dirDirection);
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material1.shininess);
    // combine results
    vec3 ambient  = dirAmbient  * texColorAmbient;
    vec3 diffuse  = dirDiffuse  * diff * texColorDiffuse;
    vec3 specular = dirSpecular * spec * texColorSpecular;
    return (ambient + diffuse + specular);
} 