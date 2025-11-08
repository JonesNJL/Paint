#ifndef SSBO_CLASS_H
#define SSBO_CLASS_H

#include <glad/glad.h>

class SSBO
{
	public:
		GLuint ID;

		SSBO(unsigned int* data, GLsizeiptr size, int binding);

		void Bind();
		void Unbind();
		void Delete();
		void UpdateData(unsigned int* data, GLsizeiptr size);
};

#endif