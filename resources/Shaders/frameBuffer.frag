#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
//uniform float num;

void main()
{ 
    //float num = (float)screenTexture;
    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
    //FragColor = vec4(num, num, num, 1.0);

}