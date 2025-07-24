#ifndef TEST_MODEL_H
#define TEST_MODEL_H

#include "Test.h"
#include "Model.h"

class TestModel : public Test
{
public:
    TestModel(GLFWwindow *window);
    ~TestModel();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImguiRender() override;
    void OnWindowResize(GLFWwindow* window, int width, int height) override;

private:
    GLFWwindow *m_Window;
    int m_FramebufferWidth, m_FramebufferHeight;
    std::unique_ptr<Model> m_Model;
    std::unique_ptr<Shader> m_ShaderProgram;
    std::unique_ptr<Shader> m_LightShader;
    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<Mesh> m_Light;
};

#endif
