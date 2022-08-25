#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout (std140, binding = 1) uniform Time
{
    float curTime;
};

layout (std140, binding = 3) uniform ClippingPlane
{
    vec4 clippingPlane;
};

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
    gl_ClipDistance[0] = gl_in[0].gl_ClipDistance[0];
    gs_out.FragPos = gs_in[0].FragPos;
    gs_out.Normal = gs_in[0].Normal;
    gs_out.TexCoord = gs_in[0].TexCoord;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    gl_ClipDistance[0] = gl_in[1].gl_ClipDistance[0];
    gs_out.FragPos = gs_in[1].FragPos;
    gs_out.Normal = gs_in[1].Normal;
    gs_out.TexCoord = gs_in[1].TexCoord;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    gl_ClipDistance[0] = gl_in[2].gl_ClipDistance[0];
    gs_out.FragPos = gs_in[2].FragPos;
    gs_out.Normal = gs_in[2].Normal;
    gs_out.TexCoord = gs_in[2].TexCoord;
    EmitVertex();

    EndPrimitive();
}  

//layout (triangles) in;
//layout (triangle_strip, max_vertices = 5) out;
//
//layout (std140, binding = 1) uniform Time
//{
//    float curTime;
//};
//
//in VS_OUT {
//    vec3 Normal;
//    vec3 FragPos;
//    vec3 LightPos;
//    vec2 TexCoord;
//} gs_in[];  
//
//out GS_OUT {
//    vec3 Normal;
//    vec3 FragPos;
//    vec3 LightPos;
//    vec2 TexCoord;
//} gs_out;
//
//
//vec3 GetNormal()
//{
//   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
//   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
//   return normalize(cross(a, b));
//}  
//
//vec4 explode(vec4 position, vec3 normal)
//{
//    float magnitude = 2.0;
//    vec3 direction = normal * ((sin(curTime * 2) + 1.0) / 3.0) * magnitude; 
//    return position + vec4(direction, 0.0);
//} 
//
//void main() {    
//    vec3 normal = GetNormal();
//
//    gl_Position = explode(gl_in[0].gl_Position, normal);
//    gs_out.TexCoord = gs_in[0].TexCoord;
//    gs_out.Normal = normal;
//    EmitVertex();
//    gl_Position = explode(gl_in[1].gl_Position, normal);
//    gs_out.TexCoord = gs_in[1].TexCoord;
//    gs_out.Normal = normal;
//    EmitVertex();
//    gl_Position = explode(gl_in[2].gl_Position, normal);
//    gs_out.Normal = normal;
//    gs_out.TexCoord = gs_in[2].TexCoord;
//    EmitVertex();
//    EndPrimitive();
//}  