#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include "Mesh.h"
#include "imgui/imgui.h"

class Test
{
public:
    Test(GLFWwindow *window);
    virtual ~Test();

    virtual void OnUpdate(float deltaTime);
    virtual void OnRender();
    virtual void OnImguiRender();
    virtual void OnWindowResize(GLFWwindow* window, int width, int height);
    void SetFrameBufferData();
    void BindPostProcessingFrameBuffer();
    void DrawPostProcessingOnScreen();
    int FrameWidth, FrameHeight;
    bool m_PostProcessing = false;

private:
    unsigned int m_FBO, m_FrameBufferTex;
    VAO m_FrameBufferVAO;
    std::unique_ptr<Shader> m_FrameBufferShader;
};

#endif
