#ifndef EBO_HEADER_FILE
#define EBO_HEADER_FILE

#include<glad/glad.h>

class EBO
{
public:
	GLuint ID;
	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif // EBO_HEADER_FILE