#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include"ShaderClass.h"
#include"VBO.h"
#include"VAO.h"
#include"EBO.h"
#include"Texture.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    std::cout << "Framebuffer resized: " << width << "x" << height << "\n";
    glViewport(0, 0, width, height);
}

int main() 
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,     0.5f, 0.2f, 0.6f,     0.0f, 1.0f,
		0.5f, 0.5f, 0.0f,      0.7f, 0.8f, 0.4f,     1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,      0.25f, 0.7f, 0.8f,   1.0f, 0.0f
	};

	GLuint indices[] =
	{
		0,2,1,
		0,3,2
	};

	GLfloat colours[] =
	{
		0.0f, 1.0f, 1.0f,
		0.25f, 0.7f, 0.8f,
		0.5f, 0.2f, 0.6f,
		0.7f, 0.8f, 0.4f,
		0.95f, 0.1f, 0.1f,
		1.0f, 1.0f, 0.0f
	};

	GLFWwindow* window = glfwCreateWindow(800, 800, "JustOpenGL", NULL, NULL);
	if (window == NULL) {
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

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	//VBO VBO2(colours, sizeof(colours));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3* sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6* sizeof(float)));


	VBO1.Unbind();
	VAO1.Unbind();
	EBO1.Unbind();

	GLuint uniId = glGetUniformLocation(shaderProgram.ID, "scale");

	Texture texture("flower.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	GLuint uniTex0 = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(uniTex0, 0);

	// glClearColor(0.11f, 0.13f, 0.14f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT);
	// glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window)) 
	{
		glClearColor(0.11f, 0.13f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)(framebufferWidth/framebufferHeight), 0.1f, 100.0f);

		GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		std::cout << "modelLoc: " << modelLoc << std::endl;
		std::cout << "model: " << glm::to_string(model) << std::endl;

		GLint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		std::cout << "viewLoc: " << viewLoc << std::endl;
		std::cout << "view: " << glm::to_string(view) << std::endl;

		GLint projectionLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		std::cout << "projectionLoc: " << projectionLoc << std::endl;
		std::cout << "projection: " << glm::to_string(projection) << std::endl;

		glUniform1f(uniId, 0.0f);
		texture.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	VBO1.Delete();
	//VBO2.Delete();
	VAO1.Delete();
	EBO1.Delete();
	texture.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
