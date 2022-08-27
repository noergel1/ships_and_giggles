#version 430 core
in GS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
    vec4 ClipSpace;
    vec3 vecToCamera;
} fs_in;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvTexture;
const float waveStrength = 0.01f;
const float waveSpeed = 0.01f;

layout (std140, binding = 1) uniform Time
{
    float curTime;
};

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
vec3 CalcReflectionRefraction();
vec3 generateWaveSineSumImprovedNormal(sinParams _params[sineCount]);

    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-fs_in.FragPos);
    vec3 viewDir = normalize(viewPos-fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float shininess = 128.0f;
    vec3 texColorAmbient = vec3(0.0f, 0.1f, 0.5f);
    vec3 texColorDiffuse = vec3(0.0f, 0.0f, 1.0f);
    vec3 texColorSpecular = vec3(1.0f, 1.0f, 1.0f);



void main()
{
    vec3 result = CalcReflectionRefraction();
    result += CalcDirLight();
    gl_FragColor = vec4(result, 1.0f);
}

vec3 CalcReflectionRefraction()
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    vec3 viewVector = normalize(fs_in.vecToCamera);
    float reflectiveRatio = dot(viewVector, fs_in.Normal);

    float moveFactor = (curTime * waveSpeed);
    moveFactor = moveFactor - floor(moveFactor);

    vec2 ndc = fs_in.ClipSpace.xy / fs_in.ClipSpace.w; 
    //ndc = (ndc + 1.0)/2.0;
    ndc = (ndc/2.0f)+0.5f;
    vec2 reflectTexCoords = vec2(ndc.x, 1.0f-ndc.y);
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);

    vec2 distortion1 = texture(dudvTexture, vec2(fs_in.TexCoord.x + moveFactor, fs_in.TexCoord.y)).rg * 2.0f - 1.0f;
    vec2 distortion2 = texture(dudvTexture, vec2(1.0f - fs_in.TexCoord.x + moveFactor, fs_in.TexCoord.y + moveFactor)).rg * 2.0f - 1.0f;
    vec2 totalDistortion = (distortion1 + distortion2) / 2;

    reflectTexCoords += (totalDistortion*waveStrength);
    reflectTexCoords = clamp(reflectTexCoords, 0.001, 0.999);
    refractTexCoords += (totalDistortion*waveStrength);
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

    result = mix(texture(reflectionTexture, reflectTexCoords).xyz, texture(refractionTexture, refractTexCoords).xyz, reflectiveRatio);
    result = mix(result, vec3(0.0f, 0.3f, 0.5f), 0.2f);

    return result;
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
