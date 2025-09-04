#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures, unsigned int instanceCount, std::vector<glm::mat4> instanceMatrices)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->instanceCount = instanceCount;
    
    VAO.Bind();
    VBO vbo(vertices);
    EBO EBO(indices);
    
    
    VAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    VAO.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float)));
    VAO.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));
    VAO.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float)));
    
    if (instanceCount > 1)
    {
        VBO instanceVbo(instanceMatrices);
        VAO.LinkAttrib(instanceVbo, 4, 4, GL_FLOAT, sizeof(glm::mat4), (void *)0);
        VAO.LinkAttrib(instanceVbo, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void *)(4 * sizeof(float)));
        VAO.LinkAttrib(instanceVbo, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void *)(8 * sizeof(float)));
        VAO.LinkAttrib(instanceVbo, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void *)(12 * sizeof(float)));
        
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);

        instanceVbo.Unbind();
    }

    vbo.Unbind();
    VAO.Unbind();
    EBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera)
{
    shader.Activate();
    VAO.Bind();

    int diffuseNum = 0;
    int specularNum = 0;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string num;
        std::string type = textures[i].type;
        if (type == "diffuse")
        {
            num = std::to_string(diffuseNum++);
        }
        else if (type == "specular")
        {
            num = std::to_string(specularNum++);
        }
        textures[i].TexSlot(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }
    camera.SetUniforms(shader);

    if (instanceCount == 1)
    {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);
    }

}

void Mesh::TexSlot(Shader &shader)
{
    // int diffuseNum = 0;
    // int specularNum = 0;
    // for (unsigned int i = 0; i < textures.size(); i++)
    // {
    //     std::string num;
    //     std::string type = textures[i].type;
    //     if (type == "diffuse")
    //     {
    //         num = std::to_string(diffuseNum++);
    //     }
    //     else if (type == "specular")
    //     {
    //         num = std::to_string(specularNum++);
    //     }
    //     textures[i].TexSlot(shader, (type + num).c_str());
    // }
}