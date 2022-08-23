#version 430 core
out vec4 FragColor;

in GS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
} fs_in;



// Directional Light
layout (std140, binding = 2) uniform dirLight
{
    vec3 dirDirection;
    vec3 dirAmbient;
    vec3 dirDiffuse;
    vec3 dirSpecular;
};


struct sinParams{
        float amplitude;
        vec2 direction;
        float wavelength;
        float frequency;
        float speed;
};
const unsigned int sineCount = 2;

// camera position
uniform vec3 viewPos;

// Calculation Functions
vec3 CalcDirLight();
vec3 generateWaveSineSumImprovedNormal(sinParams _params[sineCount]);

    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-fs_in.FragPos);
    vec3 viewDir = normalize(viewPos-fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float shininess = 16.0f;
    vec3 texColorAmbient = vec3(0.0f, 0.1f, 0.5f);
    vec3 texColorDiffuse = vec3(0.0f, 0.0f, 1.0f);
    vec3 texColorSpecular = vec3(1.0f, 1.0f, 1.0f);



void main()
{



    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    result += CalcDirLight();
    FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight()
{
    vec3 lightDir = normalize(-dirDirection);
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = dirAmbient  * texColorAmbient;
    vec3 diffuse  = dirDiffuse  * diff * texColorDiffuse;
    vec3 specular = dirSpecular * spec * texColorSpecular;
    return (ambient + diffuse + specular);
}  
//vec3 generateWaveSineSumImprovedNormal(sinParams _params[sineCount])
//{
//        vec2 pos = vec2(aPos.x, aPos.z);
//        vec3 normal = vec3(0.0f, 1.0f, 0.0f);
//
//        for(int i=0; i<sineCount; i++)
//        {
//            sinParams curParams = _params[i];
//            normal.x += sineExponent * curParams.direction.x * curParams.frequency * curParams.amplitude * 
//                        pow((sin(dot(curParams.direction, pos) * curParams.frequency + curTime * curParams.speed)+1)/2, sineExponent-1)
//                        * cos(dot(curParams.direction, pos) * curParams.frequency + curTime * curParams.speed);
//
//            normal.z += sineExponent * curParams.direction.y * curParams.frequency * curParams.amplitude * 
//                        pow((sin(dot(curParams.direction, pos) * curParams.frequency + curTime * curParams.speed)+1)/2, sineExponent-1)
//                        * cos(dot(curParams.direction, pos) * curParams.frequency + curTime * curParams.speed);
//        }
//
//        return vec3(-normal.x, normal.y, -normal.z);
//
//}
