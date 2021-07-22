#ifndef SHADERCLASS_HEADER_FILE
#define SHADERCLASS_HEADER_FILE

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
	public:
		GLuint ID;
		Shader(const char* vertexFile, const char* fragmentFile);

		void Activate();
		void Delete();

		int GetUniformLocation(const GLchar* name);
	private:
		void compileErrors(unsigned int shader, const char* type);

};

#endif // SHADERCLASS_HEADER_FILE