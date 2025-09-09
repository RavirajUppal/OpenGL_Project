#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include "Mesh.h"
#include "Light.h"
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
    virtual void RenderShadowMap(const std::unique_ptr<Shader>& shader) {};

    void SetFrameBufferData();
    void BindPostProcessingFrameBuffer();
    void DrawPostProcessingOnScreen();
    int FrameWidth, FrameHeight;
    bool m_Shadow = false;
    unsigned int m_DepthTexture;
    VAO m_FrameBufferVAO;

protected:
    std::unique_ptr<Light> m_Light;
    
private:
    int m_ShadowWidth = 1024, m_ShadowHeight = 1024;
    bool m_MSAA, m_PostProcessing = false;
    int m_CurrentPostProcess = 0;
    unsigned int m_MultisamplingFBO, m_PostProcessingFBO, m_FrameBufferTex;
    unsigned int m_ShadowMapFBO;
    std::unique_ptr<Shader> m_PostProcessShader;
    std::unique_ptr<Shader> m_ShadowMapShader;
    std::unique_ptr<Shader> m_DebugOutputShader;
    void RenderShadowPass();
    void ConfigureShaderAndMatrix();
};

#endif
