#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Mesh.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	std::cout << "Framebuffer resized: " << width << "x" << height << "\n";
	glViewport(0, 0, width, height);
}

// GLfloat vertices[] =
// { //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
// 	-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
// 	-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
// 	 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
// 	 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
// };

Vertex vertices[] =
	{ //               COORDINATES           /            NORMALS          /           COLORS         /       TEXTURE COORDINATES    //
		Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}};

GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3};

Vertex lightVertices[] =
	{ //     COORDINATES     //
		Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
		Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
		Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}};

// GLfloat lightvertices[] =
// {
// 	-0.1f, -0.1f, -0.1f,
// 	-0.1f, -0.1f, 0.1f,
// 	-0.1f, 0.1f, 0.1f,
// 	-0.1f, 0.1f, -0.1f,
// 	0.1f, -0.1f, -0.1f,
// 	0.1f, -0.1f, 0.1f,
// 	0.1f, 0.1f, 0.1f,
// 	0.1f, 0.1f, -0.1f
// };

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

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "JustOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	// glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gladLoadGL();

	int framebufferWidth, framebufferHeight;
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	Texture textures[] = {
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)};

	Shader shaderProgram("default.vert", "default.frag");

	std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> indi(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

	Mesh floor(verts, indi, tex);
	floor.TexSlot(shaderProgram);

	// VAO VAO1;
	// VAO1.Bind();
	// VBO VBO1(vertices, sizeof(vertices));
	// EBO EBO1(indices, sizeof(indices));

	// VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void *)0);
	// VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	// VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	// VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void *)(8 * sizeof(float)));

	// VBO1.Unbind();
	// VAO1.Unbind();
	// EBO1.Unbind();

	// Texture planks("planks.png", "GL_TEXTURE_2D", 0, GL_RGBA, GL_UNSIGNED_BYTE);
	// planks.TexSlot(shaderProgram, 0, "tex0");

	// Texture planksSpec("planksSpec.png", "GL_TEXTURE_2D", 1, GL_RED, GL_UNSIGNED_BYTE);
	// planksSpec.TexSlot(shaderProgram, 1, "tex1");

	Shader lightShader("light.vert", "light.frag");

	std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightIndi(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

	Mesh light(lightVerts, lightIndi, tex);
	light.TexSlot(lightShader);

	// VAO LightVAO;
	// LightVAO.Bind();
	// VBO VBO2(lightvertices, sizeof(lightvertices));
	// EBO EBO2(lightIndices, sizeof(lightIndices));

	// LightVAO.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);

	// VBO2.Unbind();
	// LightVAO.Unbind();
	// EBO2.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	glm::vec3 pyramidPos = glm::vec3(0.0f, -0.4f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Camera camera(framebufferWidth, framebufferHeight, glm::vec3(0.0f, 0.0f, 2.0f));
	glEnable(GL_DEPTH_TEST);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	// const char* glsl_version = "#version 150";
	ImGui_ImplOpenGL3_Init();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.11f, 0.13f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(); // Show demo window! :)


		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		glViewport(0, 0, framebufferWidth, framebufferHeight);

		camera.Inputs(window);
		camera.UpdateMatrix(60.0f, 0.1f, 100.0f, framebufferWidth, framebufferHeight);

		floor.Draw(shaderProgram, camera);

		// shaderProgram.Activate();
		// glUniform3f(glGetUniformLocation(shaderProgram.ID, "CameraPos"), camera.position.x, camera.position.y, camera.position.z);

		// camera.Matrix(shaderProgram, "cameraMatrix");
		// planks.Bind();
		// planksSpec.Bind();

		// VAO1.Bind();
		// glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		light.Draw(lightShader, camera);

		// lightShader.Activate();
		// camera.Matrix(lightShader, "cameraMatrix");
		// LightVAO.Bind();
		// glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// glDrawArrays(GL_TRIANGLES, 0, 6);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// VBO1.Delete();
	// VAO1.Delete();
	// EBO1.Delete();
	// planks.Delete();
	// planksSpec.Delete();

	shaderProgram.Delete();

	// VBO2.Delete();
	// LightVAO.Delete();
	// EBO2.Delete();
	lightShader.Delete();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
