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


// Directional Light
//struct DirLight {
//    vec3 direction;
//  
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};  
//uniform DirLight dirLight;



// Point Lights
//struct PointLight {    
//    vec3 position;
//    
//    float constant;
//    float linear;
//    float quadratic;  
//
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};  
//#define NR_POINT_LIGHTS 4  
//uniform PointLight pointLights[NR_POINT_LIGHTS];
//
//
//// Spotlight
//struct SpotLight {
//    vec3  position;
//    vec3  direction;
//    float innerCutOff;
//    float outerCutOff;
//
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//
//    float constant;
//    float linear;
//    float quadratic;
//}; 
//uniform SpotLight spotLight;

// camera position
layout (std140, binding = 4) uniform Camera
{
    vec3 cameraPosition;
};

// Calculation Functions
vec3 CalcDirLight();
//vec3 CalcPointLight(PointLight light);  
//vec3 CalcSpotLight(SpotLight light);

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
//
////    for(int i = 0; i < NR_POINT_LIGHTS; i++)
////    {
////        result += CalcPointLight(pointLights[i]);
////    }
//
//
////    result += CalcSpotLight(spotLight);
//
//        float width = 800, 
//          height = 600;
//
//    vec4 coord = gl_FragCoord;
//
//    float x = coord.x / width;
//    float y = coord.y / height;
//    
//    float xDiff = (x - 0.5f); 
//    float yDiff = (y - 0.5f);
//    float avgDiff = (abs(xDiff) + abs(yDiff));


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

//vec3 CalcPointLight(PointLight light)
//{
//    vec3 lightDir = normalize(light.position - fs_in.FragPos);
//    // diffuse shading
//    float diff = max(dot(norm, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material1.shininess);
//    // attenuation
//    float distance    = length(light.position - fs_in.FragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + 
//  			     light.quadratic * (distance * distance));    
//    // combine results
//    vec3 ambient  = light.ambient  * texColorAmbient;
//    vec3 diffuse  = light.diffuse  * diff * diff * texColorDiffuse;
//    vec3 specular = light.specular * spec * spec * texColorSpecular;
//    ambient  *= attenuation;
//    diffuse  *= attenuation;
//    specular *= attenuation;
//    return (ambient + diffuse + specular);
//} 
//
//
//vec3 CalcSpotLight(SpotLight light)
//{
//
//
//
//    vec3 viewDir = normalize(-fs_in.FragPos);
//    vec3 reflectDir = reflect(light.direction, norm);
//    vec3 lightDir = normalize(light.position - fs_in.FragPos);
//
//     
//
//    
//    // ambient
//    vec3 ambient = light.ambient * texColorAmbient;
//    
//    // diffuse 
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = light.diffuse * diff * texColorDiffuse;  
//    
//    // specular
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material1.shininess);
//    vec3 specular = light.specular * spec * texColorSpecular;  
//    
//    // spotlight (soft edges)
//    float theta     = dot(lightDir, normalize(-light.direction));
//    float epsilon   = light.innerCutOff - light.outerCutOff;
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//    diffuse  *= intensity;
//    specular *= intensity;
//    
//    // attenuation
//    float distance    = length(light.position - fs_in.FragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//    ambient  *= attenuation; 
//    diffuse   *= attenuation;
//    specular *= attenuation;   
//        
//    vec3 result = ambient + diffuse + specular;
//
//
//
//    return result;
//}