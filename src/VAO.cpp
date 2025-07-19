#include"VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

VAO::~VAO()
{
	Delete();
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint count, GLenum type, GLsizeiptr size, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, count, type, GL_FALSE, size, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}