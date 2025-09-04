#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int mode;

const float offset = 1.0 / 300.0; 

vec4 GetTexture();
vec4 GetInverseColor();
vec4 GetGrayImage();
vec4 GetKernelApplied(float kernel[9]);


vec4 GetTexture()
{
    return texture(screenTexture, TexCoords);
}

vec4 GetInverseColor()
{
    return vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}

vec4 GetGrayImage()
{
    vec4 color = texture(screenTexture, TexCoords);
    float average = (color.r + color.g + color.b) / 3.0;
    return vec4(average, average, average, 1.0);
}

vec4 GetKernelApplied(float kernel[9])
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    return vec4(col, 1.0);
}

vec4 SelectMode()
{
    if (mode == 1)
    {
        return GetInverseColor();
    }
    else if (mode == 2)
    {
        return GetGrayImage();
    }
    else if (mode == 3)
    {
        float sharpenKernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
        );
        return GetKernelApplied(sharpenKernel);
    }
      else if (mode == 4)
    {
        float blurKernel2[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
        );
        return GetKernelApplied(blurKernel2);
    }
    else if (mode == 5)
    {
        float edgeKernel[9] = float[](
        1,  1,  1,
        1, -8,  1,
        1,  1,  1
        );
        return GetKernelApplied(edgeKernel);
    }
    else
    {
        return GetTexture();
    }
}

void main()
{    
    FragColor = SelectMode();
}