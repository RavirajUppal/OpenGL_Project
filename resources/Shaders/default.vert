#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

uniform mat4 model;
uniform mat4 cameraMatrix;

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

#ifdef USE_GEOMETRY
out DATA {
	vec3 currPos;
    vec3 normal;
    vec3 color;
    vec2 texCoord;
    mat4 projection;
} data_out;
#else
out vec3 currPos;
out vec3 normal;
out vec3 color;
out vec2 texCoord;
#endif

void main()
{
	//currPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
	//normal = aNormal;
	//color = aColor;
	////texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	//texCoord = aTex;
	
	//gl_Position = cameraMatrix * vec4(currPos, 1.0f);

#ifdef USE_GEOMETRY
	data_out.currPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
	data_out.normal = aNormal;
	data_out.color = aColor;
	data_out.texCoord = aTex;
	data_out.projection = cameraMatrix;
	gl_Position = model * translation * rotation * scale * vec4(aPos, 1.0f);
#else
	currPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
	normal = aNormal;
	color = aColor;
	texCoord = aTex;
	gl_Position = cameraMatrix * vec4(currPos, 1.0f);
#endif
}