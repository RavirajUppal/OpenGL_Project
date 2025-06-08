#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 texCoord;
out vec3 normal;
out vec3 currPos;

uniform mat4 model;
uniform mat4 cameraMatrix;

void main()
{
	currPos = vec3(model * vec4(aPos, 1.0f));
	//gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0f);
	gl_Position = cameraMatrix * model * vec4(currPos, 1.0f);
	color = aColor;
	texCoord = aTex;
	normal = aNormal;
}