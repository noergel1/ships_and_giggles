#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
} gs_in[];  

out GS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
} gs_out;

void main() {    
    gl_Position = gl_in[0].gl_Position;
    gs_out.FragPos = gs_in[0].Normal;
    gs_out.Normal = gs_in[0].Normal;
    gs_out.TexCoord = gs_in[0].TexCoord;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    gs_out.FragPos = gs_in[1].Normal;
    gs_out.Normal = gs_in[1].Normal;
    gs_out.TexCoord = gs_in[1].TexCoord;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    gs_out.FragPos = gs_in[2].Normal;
    gs_out.Normal = gs_in[2].Normal;
    gs_out.TexCoord = gs_in[2].TexCoord;
    EmitVertex();

    EndPrimitive();
}  