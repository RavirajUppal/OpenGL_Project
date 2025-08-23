#include "Test.h"

Test::Test(GLFWwindow *window)
{
    glfwGetFramebufferSize(window, &FrameWidth, &FrameHeight);
    glViewport(0, 0, FrameWidth, FrameHeight);

    SetFrameBufferData();
}

Test::~Test()
{
}

void Test::OnUpdate(float deltaTime)
{
}

void Test::OnRender()
{
}

void Test::OnImguiRender()
{
    ImGui::Checkbox("PostProcessing", &m_PostProcessing);
}

void Test::OnWindowResize(GLFWwindow *window, int width, int height)
{
    FrameWidth = width;
    FrameHeight = height;
    glViewport(0, 0, width, height);
}

void Test::SetFrameBufferData()
{
    m_FrameBufferShader = std::make_unique<Shader>(SHADER_DIR "frameBuffer.vert", SHADER_DIR "frameBuffer.frag");
    m_FrameBufferShader->Activate();
    glUniform1i(glGetUniformLocation(m_FrameBufferShader->ID, "screenTexture"), 0);

    // glEnable(GL_CULL_FACE);
	// // Keeps front faces
	// glCullFace(GL_FRONT);

    Vertex quadVertices[] =
        {//               COORDINATES           /            NORMALS          /           COLORS         /       TEXTURE COORDINATES    //
         Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
         Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
         Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
         Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}};

    GLuint quadIndices[] =
        {
            0, 1, 2,
            0, 2, 3};

    std::vector<Vertex> vertices(quadVertices, quadVertices + sizeof(quadVertices) / sizeof(Vertex));
    std::vector<GLuint> indices(quadIndices, quadIndices + sizeof(quadIndices) / sizeof(GLuint));

    m_FrameBufferVAO.Bind();
    VBO vbo(vertices);
    EBO ebo(indices);
    m_FrameBufferVAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    m_FrameBufferVAO.LinkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float)));
    vbo.Unbind();
    m_FrameBufferVAO.Unbind();
    ebo.Unbind();

    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glGenTextures(1, &m_FrameBufferTex);
    glBindTexture(GL_TEXTURE_2D, m_FrameBufferTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FrameWidth, FrameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FrameBufferTex, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, FrameWidth, FrameHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Test::BindPostProcessingFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void Test::DrawPostProcessingOnScreen()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    m_FrameBufferShader->Activate();
    m_FrameBufferVAO.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FrameBufferTex);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
