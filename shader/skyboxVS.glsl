#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 model;
layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * mat4(mat3(view)) * model * vec4(aPos, 1.0);
    // make z- and w-component 1.0, so that the z-component is 1.0 after perspective division
    gl_Position = pos.xyww;
}  