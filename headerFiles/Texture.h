#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>
#include"ShaderClass.h"

class Texture
{
    public:
        GLuint ID;
        const char* type;
        GLuint slot;
        Texture(const char* imagePath, const char* texType, GLuint slot);

        void TexSlot(Shader& shaderProgram, const char* uniform, GLuint unit);
        void Bind();
        void UnBind();
        void Delete();
};

#endif