#version 330 core
out vec4 FragColor;

in vec3 currPos;
in vec3 normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 CameraPos;

vec4 DirectionalLight()
{
  float ambient = 0.2f;
  vec3 Normal = normalize(normal);
  vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
  float diffuse = max(dot(lightDirection, Normal), 0.0f);

  float specularLight = 0.5f;
  vec3 viewDirection = normalize(CameraPos - currPos);
  vec3 reflectionDirection = reflect(-lightDirection, Normal);
  float specAmount = pow(max(dot(reflectionDirection, viewDirection), 0.0f), 12);
  float specular = specAmount * specularLight;

  return (texture(diffuse0, texCoord)  * (diffuse + ambient) + texture(specular0, texCoord).r  * specular) * lightColor ;
}

vec4 PointLight()
{
  vec3 lightVec = lightPos - currPos;
  float dist = length(lightVec);
  float a = 0.2f;
  float b = 0.7f;
  float inten = 1.0f/(a * dist * dist + b * dist + 1.0f);
  float ambient = 0.2f;
  vec3 Normal = normalize(normal);
  vec3 lightDirection = normalize(lightVec);
  float diffuse = max(dot(lightDirection, Normal), 0.0f);

  float specularLight = 0.5f;
  vec3 viewDirection = normalize(CameraPos - currPos);
  vec3 reflectionDirection = reflect(-lightDirection, Normal);
  float specAmount = pow(max(dot(reflectionDirection, viewDirection), 0.0f), 12);
  float specular = specAmount * specularLight;

  return (texture(diffuse0, texCoord)  * (diffuse * inten + ambient) + texture(specular0, texCoord).r  * specular * inten) * lightColor ;
}

vec4 spotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 Normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currPos);
	float diffuse = max(dot(Normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(CameraPos - currPos);
	vec3 reflectionDirection = reflect(-lightDirection, Normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the currPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

void main()
{
  FragColor = spotLight();
}

