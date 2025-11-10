#include "VAO.hpp"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO VBO)
{
	VBO.Bind();
	glVertexAttribIPointer(0, 3, GL_UNSIGNED_INT, 8 * sizeof(unsigned int), (void*)(0 * sizeof(unsigned int))); // 3 Ints: Vert position
	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(1, 4, GL_UNSIGNED_INT, 8 * sizeof(unsigned int), (void*)(3 * sizeof(unsigned int))); // 4 Ints: Vert UV (2 ratios)
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 8 * sizeof(unsigned int), (void*)(7 * sizeof(unsigned int))); // 1 Int: Texture ID
	glEnableVertexAttribArray(2);
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