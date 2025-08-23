#include "TestLight.h"

TestLight::TestLight(GLFWwindow *window) : Test(window), m_Window(window)
{
    Vertex vertices[] =
        {//               COORDINATES           /            NORMALS          /           COLORS         /       TEXTURE COORDINATES    //
         Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
         Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
         Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
         Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}};

    GLuint indices[] =
        {
            0, 1, 2,
            0, 2, 3};

    Vertex lightVertices[] =
        {//     COORDINATES     //
         Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
         Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
         Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
         Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
         Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
         Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
         Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
         Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}};

    GLuint lightIndices[] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 4, 7,
            0, 7, 3,
            4, 5, 7,
            4, 6, 7,
            1, 5, 6,
            1, 6, 2,
            2, 6, 7,
            2, 7, 3,
            1, 5, 4,
            1, 4, 0};

    Texture textures[] = {
        Texture(TEXTURE_DIR "planks.png", "diffuse", 0),
        Texture(TEXTURE_DIR "planksSpec.png", "specular", 1)};

	glfwGetFramebufferSize(window, &FrameWidth, &FrameHeight);
	glViewport(0, 0, FrameWidth, FrameHeight);

    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> indi(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

    std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightIndi(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));


	m_LightShader = std::make_unique<Shader>(SHADER_DIR "light.vert", SHADER_DIR "light.frag");

	m_Light = std::make_unique<Mesh>(lightVerts, lightIndi, tex);
	// m_Light->TexSlot(*m_LightShader);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
	m_LightShader->Activate();
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(m_LightShader->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);


    m_FloorShader = std::make_unique<Shader>(SHADER_DIR "default.vert", SHADER_DIR "default.frag");
	m_Floor = std::make_unique<Mesh>(verts, indi, tex);
	// m_Floor->TexSlot(*m_FloorShader);
    glm::vec3 floorPos = glm::vec3(0.0f, -0.4f, 0.0f);
	glm::mat4 floorModel = glm::mat4(1.0f);
	floorModel = glm::translate(floorModel, floorPos);
	m_FloorShader->Activate();
	glUniformMatrix4fv(glGetUniformLocation(m_FloorShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
	glUniform4f(glGetUniformLocation(m_FloorShader->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(m_FloorShader->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1i(glGetUniformLocation(m_FloorShader->ID, "lightMode"), 0);

    glUniformMatrix4fv(glGetUniformLocation(m_FloorShader->ID, "translation"), 1, GL_FALSE, glm::value_ptr( glm::mat4(1.0f)));
	glUniformMatrix4fv(glGetUniformLocation(m_FloorShader->ID, "rotation"), 1, GL_FALSE, glm::value_ptr( glm::mat4(1.0f)));
	glUniformMatrix4fv(glGetUniformLocation(m_FloorShader->ID, "scale"), 1, GL_FALSE, glm::value_ptr( glm::mat4(1.0f)));

	m_Camera = std::make_unique<Camera>(FrameHeight, FrameHeight, glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, -0.2f, -1.0f));
}

TestLight::~TestLight()
{
}

void TestLight::OnUpdate(float deltaTime)
{
    m_Camera->Inputs(m_Window);
	m_Camera->UpdateMatrix(60.0f, 0.1f, 100.0f);
}

void TestLight::OnRender()
{
    if (m_PostProcessing)
    {
        Test::BindPostProcessingFrameBuffer();
        DrawScene();
        Test::DrawPostProcessingOnScreen();
        return;
    }
    DrawScene();
}

void TestLight::DrawScene()
{
    m_Floor->Draw(*m_FloorShader, *m_Camera);
    m_Light->Draw(*m_LightShader, *m_Camera);
}

void TestLight::OnImguiRender()
{
    if (ImGui::Button("Spot Light"))
    {
	    m_FloorShader->Activate();
        glUniform1i(glGetUniformLocation(m_FloorShader->ID, "lightMode"), 0);
    }
    if (ImGui::Button("Point Light"))
    {
	    m_FloorShader->Activate();
        glUniform1i(glGetUniformLocation(m_FloorShader->ID, "lightMode"), 1);
    }
    if (ImGui::Button("Directional Light"))
    {
	    m_FloorShader->Activate();
        glUniform1i(glGetUniformLocation(m_FloorShader->ID, "lightMode"), 2);
    }
    Test::OnImguiRender();
}

void TestLight::OnWindowResize(GLFWwindow *window, int width, int height)
{
    Test::OnWindowResize(window, width, height);
    m_Camera->OnWindowResize(width, height);
}
