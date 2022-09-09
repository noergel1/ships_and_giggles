#version 430 core
layout (location = 0) in vec3 aPos;


uniform mat4 model[100];
layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
	gl_Position = projection * view * model[gl_InstanceID] * vec4(aPos, 1.0);
}