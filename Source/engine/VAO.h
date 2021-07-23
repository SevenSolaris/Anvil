#ifndef VAO_HEADER_FILE
#define VAO_HEADER_FILE

#include<glad/glad.h>
#include "VBO.h"

class VAO
{
public:
	GLuint ID;
	VAO();

	void LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	void Bind();
	void Unbind();
	void Delete();
};

#endif // VAO_HEADER_FILE