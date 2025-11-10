#include "SSBO.hpp"

SSBO::SSBO(unsigned int* data, GLsizeiptr size, int binding)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(unsigned int), data, GL_STREAM_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, ID);
}

void SSBO::Bind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
}

void SSBO::Unbind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SSBO::Delete()
{
	glDeleteBuffers(1, &ID);
}

void SSBO::UpdateData(unsigned int* data, GLsizeiptr size)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(unsigned int), data, GL_STREAM_DRAW);
}