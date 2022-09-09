#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model[100];
layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

layout (std140, binding = 3) uniform ClippingPlane
{
    vec4 clippingPlane;
};

out VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
} vs_out;  


void main()
{
    vs_out.FragPos = vec3(model[gl_InstanceID] * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model[gl_InstanceID]))) * aNormal;  
    vs_out.TexCoord = aTexCoord;

    //gl_ClipDistance[0] = dot(inverse(model) * vec4(aPos, 1.0), clippingPlane);
    gl_ClipDistance[0] = dot(model[gl_InstanceID] * vec4(aPos, 1.0f), clippingPlane);
    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}