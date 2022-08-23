#version 430 core
layout (location = 0) in vec3 aPos;


uniform mat4 model;
layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.color = aPos;
}