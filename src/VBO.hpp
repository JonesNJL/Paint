#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO
{
	public:
		GLuint ID;

		VBO(unsigned int* vertices, GLsizeiptr size);

		void Bind();
		void Unbind();
		void Delete();
		void UpdateVertices(unsigned int* vertices, GLsizeiptr size);
};

#endif