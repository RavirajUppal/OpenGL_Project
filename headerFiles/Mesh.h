#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    unsigned int instanceCount;
    std::vector<glm::mat4> instanceMatrices;
    VAO VAO;

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures, unsigned int instanceCount = 1, std::vector<glm::mat4> instanceMatrices = {});
    void TexSlot(Shader &shader);
    void Draw
	(
		Shader& shader, 
		Camera& camera
	);
};

#endif