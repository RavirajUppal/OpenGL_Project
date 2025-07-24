#include "TestModel.h"

TestModel::TestModel(GLFWwindow *window) : Test(window), m_Window(window)
{
    glfwGetFramebufferSize(window, &m_FramebufferWidth, &m_FramebufferHeight);
	glViewport(0, 0, m_FramebufferWidth, m_FramebufferHeight);


    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

    m_ShaderProgram = std::make_unique<Shader>(SHADER_DIR "default.vert", SHADER_DIR "default.frag");

	m_ShaderProgram->Activate();
	glUniform4f(glGetUniformLocation(m_ShaderProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(m_ShaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1i(glGetUniformLocation(m_ShaderProgram->ID, "lightMode"), 2);

	m_Camera = std::make_unique<Camera>(m_FramebufferHeight, m_FramebufferHeight, glm::vec3(0.0f, 0.0f, 30.0f));

    // m_Model = std::make_unique<Model>("resources/Models/grindstone/scene.gltf");
    m_Model = std::make_unique<Model>("resources/Models/sword/scene.gltf");
}

TestModel::~TestModel()
{
}

void TestModel::OnUpdate(float deltaTime)
{
    m_Camera->Inputs(m_Window);
	m_Camera->UpdateMatrix(60.0f, 0.1f, 100.0f);
}

void TestModel::OnRender()
{
    m_Model->Draw(*m_ShaderProgram, *m_Camera);
    // m_Light->Draw(*m_LightShader, *m_Camera);
}

void TestModel::OnImguiRender()
{
}

void TestModel::OnWindowResize(GLFWwindow *window, int width, int height)
{
}
