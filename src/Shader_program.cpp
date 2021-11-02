#include "../alledge/Shader_program.h"
#include "../alledge/Shader.h"
#include <GL/glext.h>

Shader_program::Shader_program()
{
	program = glCreateProgram();
}

Shader_program::~Shader_program()
{
	glDeleteProgram(program);
	program = 0;
}

void Shader_program::Attach_shader(Shader &shader)
{
	shader.Attach_to_program(program);
}

void Shader_program::Detach_shader(Shader &shader)
{
	shader.Detach_from_program(program);
}

GLuint Shader_program::Get_glprogram()
{
	return program;
}

GLint Shader_program::Get_uniform_location(const char* name)
{
	return glGetUniformLocation(program, name); 
}

void Shader_program::Link()
{
	glLinkProgram(program);
}

void Shader_program::Use()
{
	glUseProgram(program);
}
