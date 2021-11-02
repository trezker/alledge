#include "../alledge/Shader.h"
#include <fstream>
#include <iostream>
#include <GL/glext.h>

bool loadstring(const std::string& filename, std::string &shader)
{
	std::ifstream file;
	file.open(filename.c_str(), std::ios::in);
	if(!file) return false;

	std::string temp;
	
	while (std::getline(file, temp))
	{
		shader += temp;
	}

	file.close();
	return true;
}

Shader::Shader()
:shader(0)
{
}

Shader::~Shader()
{
	glDeleteShader(shader);
	shader = 0;
}

bool Shader::Load_shader(const std::string& filename, GLenum shaderType)
{
	std::string filestring;
	if(!loadstring(filename, filestring))
	{
		return false;
	}
	const char *shader_source = filestring.c_str();

	glDeleteShader(shader); //In case this object already holds a shader.
	shader = glCreateShader(shaderType); 
	if(shader == 0)
		return false;
	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);
	GLint err;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
	if(GL_FALSE == err)
	{
		std::cout<<"Shader failed to compile"<<std::endl;
	}
}

void Shader::Attach_to_program(GLuint program)
{
	glAttachShader(program, shader);
}

void Shader::Detach_from_program(GLuint program)
{
	glDetachShader(program, shader);
}
