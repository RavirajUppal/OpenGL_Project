#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"ShaderClass.h"
#include"VBO.h"
#include"VAO.h"
#include"EBO.h"

#include<stb/stb_image.h>



int main() 
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,      0.25f, 0.7f, 0.8f,
		-0.5f, 0.5f, 0.0f,     0.5f, 0.2f, 0.6f,
		0.5f, 0.5f, 0.0f,      0.7f, 0.8f, 0.4f,
	};

	GLuint indices[] =
	{
		0,1,2,
		2,1,3
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

	gladLoadGL();

	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	//VBO VBO2(colours, sizeof(colours));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkVBO(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkVBO(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3* sizeof(float)));

	VBO1.Unbind();
	VAO1.Unbind();
	EBO1.Unbind();

	GLuint uniId = glGetUniformLocation(shaderProgram.ID, "scale");

	glClearColor(0.11f, 0.13f, 0.14f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window)) 
	{
		glClearColor(0.11f, 0.13f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		glUniform1f(uniId, 0.0f);
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
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}