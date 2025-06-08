#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currPos;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 CameraPos;

void main()
{
  float ambient = 0.2f;
  vec3 Normal = normalize(normal);
  vec3 lightDirection = normalize(lightPos - currPos);
  float diffuse = max(dot(lightDirection, Normal), 0.0f);

  float specularLight = 0.5f;
  vec3 viewDirection = normalize(CameraPos - currPos);
  vec3 reflectionDirection = reflect(-lightDirection, Normal);
  float specAmount = pow(max(dot(reflectionDirection, viewDirection), 0.0f), 100);
  float specular = specAmount * specularLight;

  FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}