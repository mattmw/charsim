#version 330

layout(triangles) in;

layout(line_strip, max_vertices=6) out;

uniform mat4 WVP;

in VertexNormal
{
    vec4 normal;
    vec4 color;
} vertex_normal[];

out vec4 vertex_color;

void main()
{
    int i;
    for (i=0; i<gl_in.length(); i++)
    {
        vec4 P = gl_in[i].gl_Position;
        vec4 N = vertex_normal[i].normal;
        
        gl_Position = WVP * P;
        vertex_color = vec4(1.0, 0.0, 0.0, 1.0);
        EmitVertex();
        
        gl_Position = WVP * (P + N * 0.2);
        vertex_color = vertex_normal[i].color;
        EmitVertex();
        
        EndPrimitive();
    }
}