#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 currPos;
out vec3 normal;
out vec3 color;
out vec2 texCoord;
out vec4 fragPosLightSpace;

in DATA{
    vec3 currPos;
    vec3 normal;
    vec3 color;
    vec2 texCoord;
    vec4 fragPosLightSpace;
    mat4 projection;
} data_in[];


void main()
{
    //vec3 vector0 = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    //vec3 vector1 = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    //vec4 surfaceNormal = vec4(normalize(cross(vector0, vector1)), 0.0f);

    for (int i = 0; i < 3; i++) {
        gl_Position = data_in[i].projection * vec4(data_in[i].currPos, 1.0);
        currPos = data_in[i].currPos;
        normal = data_in[i].normal;
        color = data_in[i].color;
        texCoord = data_in[i].texCoord;
        fragPosLightSpace = data_in[i].fragPosLightSpace;
        EmitVertex();
    }
    EndPrimitive();
}



