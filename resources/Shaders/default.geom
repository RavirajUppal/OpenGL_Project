#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 currPos;
out vec3 normal;
out vec3 color;
out vec2 texCoord;

in DATA{
    vec3 currPos;
    vec3 normal;
    vec3 color;
    vec2 texCoord;
    mat4 projection;
} data_in[];


void main()
{
    //vec3 vector0 = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    //vec3 vector1 = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    //vec4 surfaceNormal = vec4(normalize(cross(vector0, vector1)), 0.0f);

    gl_Position = data_in[0].projection * (vec4(data_in[0].currPos, 1.0f));
    currPos = data_in[0].currPos;
    normal = data_in[0].normal;
    color = data_in[0].color;
    texCoord = data_in[0].texCoord;
    EmitVertex();

    gl_Position = data_in[1].projection * (vec4(data_in[1].currPos, 1.0f));
    currPos = data_in[1].currPos;
    normal = data_in[1].normal;
    color = data_in[1].color;
    texCoord = data_in[1].texCoord;
    EmitVertex();

    gl_Position = data_in[2].projection * (vec4(data_in[2].currPos, 1.0f));
    currPos = data_in[2].currPos;
    normal = data_in[2].normal;
    color = data_in[2].color;
    texCoord = data_in[2].texCoord;
    EmitVertex();

    EndPrimitive();
}



