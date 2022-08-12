#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    // make z- and w-component 1.0, so that the z-component is 1.0 after perspective division
    gl_Position = pos.xyww;
}  