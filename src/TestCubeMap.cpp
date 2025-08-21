#include "TestCubeMap.h"

TestCubeMap::TestCubeMap(GLFWwindow *window) : Test(window), m_Window(window)
{
    Vertex cubeVertices[] =
        {
         Vertex{glm::vec3(-1.0f, -1.0f, 1.0f)},
         Vertex{glm::vec3(1.0f, -1.0f, 1.0f)},
         Vertex{glm::vec3(1.0f, -1.0f, -1.0f)},
         Vertex{glm::vec3(-1.0f, -1.0f, -1.0f)},
         Vertex{glm::vec3(-1.0f, 1.0f, 1.0f)},
         Vertex{glm::vec3(1.0f, 1.0f, 1.0f)},
         Vertex{glm::vec3(1.0f, 1.0f, -1.0f)},
         Vertex{glm::vec3(-1.0f, 1.0f, -1.0f)}};


    GLuint cubeIndices[] =
        {
            // Right
	        1, 2, 6,
	        6, 5, 1,
	        // Left
	        0, 4, 7,
	        7, 3, 0,
	        // Top
	        4, 5, 6,
	        6, 7, 4,
	        // Bottom
	        0, 3, 2,
	        2, 1, 0,
	        // Back
	        0, 1, 5,
	        5, 4, 0,
	        // Front
	        3, 7, 6,
	        6, 2, 3};

    std::string faces[] = {
        TEXTURE_DIR "skybox/right.jpg",
        TEXTURE_DIR "skybox/left.jpg",
        TEXTURE_DIR "skybox/top.jpg",
        TEXTURE_DIR "skybox/bottom.jpg",
        TEXTURE_DIR "skybox/front.jpg",
        TEXTURE_DIR "skybox/back.jpg"
    };

	std::vector<std::string> cubeFaces(faces, faces + sizeof(faces) / sizeof(std::string));
    std::vector<Vertex> cubeVerts(cubeVertices, cubeVertices + sizeof(cubeVertices) / sizeof(Vertex));
	std::vector<GLuint> cubeIndi(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));

    glfwGetFramebufferSize(window, &m_FramebufferWidth, &m_FramebufferHeight);
	glViewport(0, 0, m_FramebufferWidth, m_FramebufferHeight);

    m_SkyBoxShader = std::make_unique<Shader>(SHADER_DIR "skybox.vert", SHADER_DIR "skybox.frag");
    m_SkyBoxShader->Activate();
    glUniform1i(glGetUniformLocation(m_SkyBoxShader->ID, "cubeMap"), 0);

	m_Camera = std::make_unique<Camera>(m_FramebufferHeight, m_FramebufferHeight, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    m_SkyBoxVAO.Bind();
    VBO VBO(cubeVerts);
    EBO EBO(cubeIndi);
    m_SkyBoxVAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    VBO.Unbind();
    m_SkyBoxVAO.Unbind();
    EBO.Unbind();

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    for (unsigned int i = 0; i < cubeFaces.size(); i++)
    {
        int nrChannels;
        unsigned char *data = stbi_load(cubeFaces[i].c_str(), &cubeMapWidth, &cubeMapHeight, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, cubeMapWidth, cubeMapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << cubeFaces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

TestCubeMap::~TestCubeMap()
{
}

void TestCubeMap::OnUpdate(float deltaTime)
{
    m_Camera->Inputs(m_Window);
	m_Camera->UpdateMatrix(60.0f, 0.1f, 100.0f);
}

void TestCubeMap::OnRender()
{
    glDepthMask(GL_FALSE);

    m_SkyBoxShader->Activate();
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    view = glm::mat4(glm::mat3(glm::lookAt(m_Camera->position, m_Camera->position + m_Camera->orientation, m_Camera->up)));
    projection = glm::perspective(glm::radians(60.0f), (float)m_FramebufferWidth / m_FramebufferHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(m_SkyBoxShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_SkyBoxShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    m_SkyBoxVAO.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_SkyBoxVAO.Unbind();

    glDepthMask(GL_TRUE);
}

void TestCubeMap::OnImguiRender()
{

}

void TestCubeMap::OnWindowResize(GLFWwindow *window, int width, int height)
{
    Test::OnWindowResize(window, width, height);
    m_Camera->OnWindowResize(width, height);
}
