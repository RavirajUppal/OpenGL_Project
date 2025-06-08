#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>
#include"ShaderClass.h"

class Texture
{
    public:
        GLuint ID;
        GLenum type;
        Texture(const char *image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
        void TexUnit(Shader& shaderProgram, int unit, const char* uniform);
        void Bind();
        void UnBind();
        void Delete();
};

#endif