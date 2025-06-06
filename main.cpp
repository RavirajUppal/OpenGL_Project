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

	gladLoadGL();

	glViewport(0, 0, 800, 800);

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

	int WidthImg, HeightImg, ColCh;
	unsigned char* bytes = stbi_load("flower.png", &WidthImg, &HeightImg, &ColCh, 0);
	
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WidthImg, HeightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

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
		glUniform1f(uniId, 0.0f);
		glBindTexture(GL_TEXTURE_2D, texture);
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