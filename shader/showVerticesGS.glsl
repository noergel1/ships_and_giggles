#version 430 core

layout (triangles) in;
layout (points, max_vertices = 3) out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
    gl_Position = projection * gl_in[1].gl_Position;
    EmitVertex();
    EndPrimitive();
    gl_Position = projection * gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();

}  