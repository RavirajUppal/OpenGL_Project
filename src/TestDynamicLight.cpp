#include "TestDynamicLight.h"

unsigned int depthMapFBO, depthMap;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

std::vector<Vertex> GenerateTheCube(float size = 1.0f)
{
    std::vector<Vertex> vertices;
    vertices.reserve(24); // 6 faces * 4 vertices

    // Directions for cube faces
    glm::vec3 normals[6] = {
        {0, 0, 1},  // Front
        {0, 0, -1}, // Back
        {-1, 0, 0}, // Left
        {1, 0, 0},  // Right
        {0, 1, 0},  // Top
        {0, -1, 0}  // Bottom
    };

    // Each face defined in CCW order
    glm::vec3 faceCoords[6][4] = {
        {{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}},     // Front
        {{-1, -1, -1}, {-1, 1, -1}, {1, 1, -1}, {1, -1, -1}}, // Back
        {{-1, -1, -1}, {-1, -1, 1}, {-1, 1, 1}, {-1, 1, -1}}, // Left
        {{1, -1, -1}, {1, 1, -1}, {1, 1, 1}, {1, -1, 1}},     // Right
        {{-1, 1, -1}, {-1, 1, 1}, {1, 1, 1}, {1, 1, -1}},     // Top
        {{-1, -1, -1}, {1, -1, -1}, {1, -1, 1}, {-1, -1, 1}}  // Bottom
    };

    // Texture coordinates (same for all faces)
    glm::vec2 uvs[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}};

    for (int f = 0; f < 6; f++)
    { // 6 faces
        for (int v = 0; v < 4; v++)
        { // 4 vertices per face
            glm::vec3 pos = faceCoords[f][v] * size;
            vertices.push_back(Vertex{pos, normals[f], glm::vec3(1.0f), uvs[v]});
        }
    }

    return vertices;
}

std::vector<GLuint> GenerateTheCubeIndices()
{
    std::vector<GLuint> indices;
    indices.reserve(36);

    for (int f = 0; f < 6; f++)
    {
        GLuint start = f * 4;
        indices.insert(indices.end(), {start, start + 1, start + 2,
                                       start, start + 2, start + 3});
    }

    return indices;
}

std::vector<glm::vec3> allcubePositions = {
    {0.0f, 2.0f, 0.0f},
    {2.0f, 0.5f, -3.0f},
    {-1.5f, 1.0f, -2.5f},
    {3.0f, 2.5f, -1.0f}
};

void InitShadowMap()
{
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 TestDynamicLight::GetLightSpaceMatrix()
{
    // glm::vec3 lightDir = glm::normalize(glm::vec3(-2.0f, -4.0f, -1.0f)); // your dir light direction

    // glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
    // glm::mat4 lightView = glm::lookAt(-lightDir * 10.0f, // position backwards along light direction
    //                                   glm::vec3(0.0f),   // look at scene center
    //                                   glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightProjection= glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 10.0f);
    glm::mat4 lightView = glm::lookAt(m_Light->GetPosition(), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    return lightProjection * lightView;
}

void TestDynamicLight::RenderTheShadowMap()
{
    glm::mat4 lightSpaceMatrix = GetLightSpaceMatrix();
    std::unique_ptr<Shader> shadowShader = std::make_unique<Shader>(SHADER_DIR "shadowMap.vert", SHADER_DIR "shadowMap.frag");
    shadowShader->Activate();
    // glUniformMatrix4fv(glGetUniformLocation(shadowShader->ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    shadowShader->SetMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // draw your scene with shadowShader
    // glm::mat4 model = glm::mat4(1.0f);
    m_Floor->Draw(*shadowShader, *m_Camera);

    for (const auto& pos : allcubePositions) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        m_Cube->Draw(*shadowShader, *m_Camera, model);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


TestDynamicLight::TestDynamicLight(GLFWwindow *window) : Test(window), m_Window(window)
{
    Vertex vertices[] =
        {//               COORDINATES           /            NORMALS          /           COLORS         /       TEXTURE COORDINATES    //
         Vertex{glm::vec3(-5.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
         Vertex{glm::vec3(-5.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
         Vertex{glm::vec3(5.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
         Vertex{glm::vec3(5.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}};

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

    //Light
    m_LightShader = std::make_unique<Shader>(SHADER_DIR "light.vert", SHADER_DIR "light.frag");
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(m_LightPos[0], m_LightPos[1], m_LightPos[2]);
    m_Light = std::make_unique<Light>(lightPos, lightColor);
    m_Light->m_Mesh = std::make_unique<Mesh>(lightVerts, lightIndi, tex);
    m_LightShader->Activate();
    glUniform4f(glGetUniformLocation(m_LightShader->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    // m_Light = std::make_unique<Mesh>(lightVerts, lightIndi, tex);

    //Floor
    m_FloorShader = std::make_unique<Shader>(SHADER_DIR "default.vert", SHADER_DIR "default.frag", SHADER_DIR "default.geom");
    m_FloorShader->Activate();
    glUniform4f(glGetUniformLocation(m_FloorShader->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(m_FloorShader->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1i(glGetUniformLocation(m_FloorShader->ID, "lightMode"), 0);
    m_Floor = std::make_unique<Mesh>(verts, indi, tex);

    //Camera
    m_Camera = std::make_unique<Camera>(FrameHeight, FrameHeight, glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3(0.0f, -0.5f, -1.0f));

    std::vector<Vertex> CubeVerts = GenerateTheCube(0.2f);
    std::vector<GLuint> CubeIndi = GenerateTheCubeIndices();

    m_Cube = std::make_unique<Mesh>(CubeVerts, CubeIndi, tex);

    InitShadowMap();
}

TestDynamicLight::~TestDynamicLight()
{
}

void TestDynamicLight::OnUpdate(float deltaTime)
{
    // float time = glfwGetTime();
    // glm::vec3 lightPos = glm::vec3(1.0f * sin(time), 1.0f, 1.0f * cos(time));

    m_Camera->Inputs(m_Window);
    m_Camera->UpdateMatrix(60.0f, 0.1f, 100.0f);
}

void TestDynamicLight::OnRender()
{
    // Test::BindPostProcessingFrameBuffer();
    RenderTheShadowMap();

    glViewport(0, 0, FrameWidth, FrameHeight);
    std::unique_ptr<Shader> m_DebugOutputShader = std::make_unique<Shader>(SHADER_DIR "depthDebug.vert", SHADER_DIR "depthDebug.frag");
    m_DebugOutputShader->Activate();
    m_DebugOutputShader->SetInt1("depthMap", 2);
    glDisable(GL_DEPTH_TEST);
    m_DebugOutputShader->Activate();
    m_FrameBufferVAO.Bind();
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);


    // DrawScene();
    // Test::DrawPostProcessingOnScreen();
}

void TestDynamicLight::DrawScene()
{
    glViewport(0, 0, FrameWidth, FrameHeight);

    glm::vec3 lightPos = m_Light->GetPosition();
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    m_Light->m_Mesh->Draw(*m_LightShader, *m_Camera, lightModel);

    // m_FloorShader->Activate();
    // glUniform3f(glGetUniformLocation(m_FloorShader->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    // m_Floor->Draw(*m_FloorShader, *m_Camera);

    m_FloorShader->Activate();
    glm::mat4 lightSpace = GetLightSpaceMatrix();
    // glUniformMatrix4fv(glGetUniformLocation(m_FloorShader->ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));
    // glUniform3f(glGetUniformLocation(m_FloorShader->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    m_FloorShader->SetMat4("lightSpaceMatrix", glm::value_ptr(lightSpace));
    m_FloorShader->SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);

    // bind shadow texture
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glUniform1i(glGetUniformLocation(m_FloorShader->ID, "shadowMap"), 3);

    m_Floor->Draw(*m_FloorShader, *m_Camera);


    for (const auto& pos : allcubePositions) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        m_Cube->Draw(*m_FloorShader, *m_Camera, model);
    }
}

void TestDynamicLight::OnImguiRender()
{
    if (ImGui::Button("Directional Light"))
    {
        m_FloorShader->Activate();
        glUniform1i(glGetUniformLocation(m_FloorShader->ID, "lightMode"), 0);
    }
    if (ImGui::Button("Point Light"))
    {
        m_FloorShader->Activate();
        glUniform1i(glGetUniformLocation(m_FloorShader->ID, "lightMode"), 1);
    }
    if (ImGui::Button("Spot Light"))
    {
        m_FloorShader->Activate();
        glUniform1i(glGetUniformLocation(m_FloorShader->ID, "lightMode"), 2);
    }
    if (ImGui::DragFloat3("Position", m_LightPos, 0.1f, -5.0f, 5.0f))
    {
        m_Light->SetPosition(glm::vec3(m_LightPos[0], m_LightPos[1], m_LightPos[2]));
    }
    Test::OnImguiRender();
}

void TestDynamicLight::OnWindowResize(GLFWwindow *window, int width, int height)
{
    Test::OnWindowResize(window, width, height);
    m_Camera->OnWindowResize(width, height);
}
