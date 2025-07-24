#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    VAO.Bind();
    VBO VBO1(vertices);
    EBO EBO1(indices);

    VAO.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    VAO.LinkAttrib(VBO1, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO1, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));
    VAO.LinkAttrib(VBO1, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float)));

    VBO1.Unbind();
    VAO.Unbind();
    EBO1.Unbind();
}

void Mesh::Draw(Shader &shader, Camera &camera)
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

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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