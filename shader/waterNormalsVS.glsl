#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;


out VS_OUT {
    vec3 normal;
} vs_out;


uniform mat4 model;
layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

layout (std140, binding = 1) uniform Time
{
    float curTime;
};

    struct sinParams{
        float amplitude;
        vec2 direction;
        float wavelength;
        float frequency;
        float speed;
    };

    const unsigned int sineCount = 2;
    const unsigned int sineExponent = 5;

//function declarations
vec3 generateWaveSineSumImproved(sinParams _params[sineCount]);
vec3 generateWaveSineSumImprovedNormal(sinParams _params[sineCount]);


void main()
{
    float amplitude = 0.05f;
    vec2 direction = vec2(0.5f, 0.5f);
    float wavelength = 0.1f;
    float frequency = 2/wavelength;
    float speed = 0.05f * frequency;

    sinParams sinFuncs[sineCount];
    sinFuncs[0] = sinParams(
    /* Amplitude  */ amplitude,
    /* Direction  */ direction,
    /* Wavelength */ wavelength,
    /* Frequency  */ frequency, 
    /* Speed      */ speed
    );

    amplitude   = 0.07f;
    direction   = vec2(0.75f, 0.25f);
    wavelength  = 0.07f;
    frequency   = 2/wavelength;
    speed       = 0.02f * frequency;

    sinFuncs[1] = sinParams(
    /* Amplitude  */ amplitude,
    /* Direction  */ direction,
    /* Wavelength */ wavelength,
    /* Frequency  */ frequency, 
    /* Speed      */ speed
    );

    vec3 FragPos = vec3(model * vec4(generateWaveSineSumImproved(sinFuncs), 1.0));
    gl_Position = view * vec4(FragPos, 1.0);

    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(generateWaveSineSumImprovedNormal(sinFuncs));

}

// algorithms taken from
// https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models

vec3 generateWaveSineSumImproved(sinParams _params[sineCount])
{
        vec2 pos = vec2(aPos.x, aPos.z);
        float vertexHeight = 0.0f;

        for(int i=0; i<sineCount; i++)
        {
            sinParams curParams = _params[i];
            vertexHeight += 2* _params[i].amplitude * pow(    (sin(dot(curParams.direction, pos) * curParams.frequency + curTime * curParams.speed)+1)/2, sineExponent    );
        }

        return vec3(aPos.x, vertexHeight/sineCount, aPos.z);

}

vec3 generateWaveSineSumImprovedNormal(sinParams _params[sineCount])
{
        vec2 pos = vec2(aPos.x, aPos.z);
        vec3 normal = vec3(0.0f, 1.0f, 0.0f);

        for(int i=0; i<sineCount; i++)
        {
            sinParams curParams = _params[i];
            normal.x += sineExponent * curParams.direction.x * curParams.frequency * curParams.amplitude * 
                        pow((sin(dot(curParams.direction, pos) * curParams.frequency + curTime * curParams.speed)+1)/2, sineExponent-1)
                        * cos(dot(curParams.direction, pos) * curParams.frequency + curTime * curParams.speed);

            normal.z += sineExponent * curParams.direction.y * curParams.frequency * curParams.amplitude * 
                        pow((sin(dot(curParams.direction, pos) * curParams.frequency + curTime * curParams.speed)+1)/2, sineExponent-1)
                        * cos(dot(curParams.direction, pos) * curParams.frequency + curTime * curParams.speed);
        }

        return vec3(-normal.x, normal.y, -normal.z);

}