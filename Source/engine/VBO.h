#ifndef VBO_HEADER_FILE
#define VBO_HEADER_FILE

#include<glad/glad.h>

class VBO
{
	public:
		GLuint ID;
		VBO(GLfloat* vertices, GLsizeiptr size);

		void Bind();
		void Unbind();
		void Delete();
};

#endif // VBO_HEADER_FILE