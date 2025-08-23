#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in DATA{
    vec3 currPos;
    vec3 normal;
    vec3 color;
    vec2 texCoord;
    mat4 projection;
} data_in[];

void main()
{
    for (int i = 0; i < 3; i++)
    {
        gl_Position = data_in[i].projection * (vec4(data_in[i].currPos, 1.0f));
        EmitVertex();

        gl_Position = data_in[i].projection * (vec4(data_in[i].currPos, 1.0f) + (vec4(data_in[i].normal, 0.0f)));
        EmitVertex();

        EndPrimitive();
    }
}