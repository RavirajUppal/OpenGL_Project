#include "TestModel.h"

TestModel::TestModel(GLFWwindow *window) : Test(window), m_Window(window)
{
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glfwGetFramebufferSize(window, &m_FramebufferWidth, &m_FramebufferHeight);
	glViewport(0, 0, m_FramebufferWidth, m_FramebufferHeight);


    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.5f, 0.5f);

    m_ShaderProgram = std::make_unique<Shader>(SHADER_DIR "default.vert", SHADER_DIR "default.frag");

	m_ShaderProgram->Activate();
	glUniform4f(glGetUniformLocation(m_ShaderProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(m_ShaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1i(glGetUniformLocation(m_ShaderProgram->ID, "lightMode"), 2);

    m_OutliningShader = std::make_unique<Shader>(SHADER_DIR "outlining.vert", SHADER_DIR "outlining.frag");

	m_Camera = std::make_unique<Camera>(m_FramebufferHeight, m_FramebufferHeight, glm::vec3(0.0f, 0.0f, 35.0f));

    // m_Model = std::make_unique<Model>("resources/Models/grindstone/scene.gltf");
    m_Model = std::make_unique<Model>("resources/Models/sword/scene.gltf");
    // m_Model = std::make_unique<Model>("resources/Models/airplane/scene.gltf");
    // m_Model = std::make_unique<Model>("resources/Models/statue/scene.gltf");
    // m_Model = std::make_unique<Model>("resources/Models/crow/scene.gltf");
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

    glStencilFunc(GL_ALWAYS, 1, 0xFF); 
    glStencilMask(0xFF); 

    m_Model->Draw(*m_ShaderProgram, *m_Camera);
    
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); 
    glDisable(GL_DEPTH_TEST);

    m_OutliningShader->Activate();
	glUniform1f(glGetUniformLocation(m_OutliningShader->ID, "outlining"), 0.08f);

    m_Model->Draw(*m_OutliningShader, *m_Camera);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST); 
}

void TestModel::OnImguiRender()
{
}

void TestModel::OnWindowResize(GLFWwindow *window, int width, int height)
{
    Test::OnWindowResize(window, width, height);
    m_Camera->OnWindowResize(width, height);
}
