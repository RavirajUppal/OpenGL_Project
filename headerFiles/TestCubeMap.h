#ifndef TESTCUBEMAP
#define TESTCUBEMAP

#include "Test.h"

class TestCubeMap : public Test
{
public:
    TestCubeMap(GLFWwindow *window);
    ~TestCubeMap();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImguiRender() override;
    void OnWindowResize(GLFWwindow* window, int width, int height) override;

private:
    GLFWwindow *m_Window;
    GLuint ID;
    int cubeMapWidth, cubeMapHeight;
    VAO m_SkyBoxVAO;
    std::unique_ptr<Shader> m_SkyBoxShader;
    std::unique_ptr<Camera> m_Camera;
    void DrawSkyBox();
};

#endif