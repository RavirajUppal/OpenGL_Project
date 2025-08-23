#ifndef TESTLIGHT_CLASS_H
#define TESTLIGHT_CLASS_H

#include "Test.h"
// #include "imgui/imgui.h"

class TestLight : public Test
{
public:
    TestLight(GLFWwindow *window);
    ~TestLight();

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
    std::unique_ptr<Mesh> m_Light;
    void DrawScene();
};

#endif