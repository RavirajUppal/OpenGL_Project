#ifndef TEST_DYNAMIC_LIGHT_CLASS_H
#define TEST_DYNAMIC_LIGHT_CLASS_H

#include "Test.h"

class TestDynamicLight : public Test
{
public:
    TestDynamicLight(GLFWwindow *window);
    ~TestDynamicLight();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImguiRender() override;
    void OnWindowResize(GLFWwindow* window, int width, int height) override;

private:
    GLFWwindow *m_Window;
    std::unique_ptr<Shader> m_FloorShader;
    std::unique_ptr<Shader> m_LightShader;
    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<Mesh> m_Floor;
    float m_LightPos[3] = {0.5f, 4.0f, 0.5f};
    void DrawScene();
};

#endif