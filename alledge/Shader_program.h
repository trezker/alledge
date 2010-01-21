#ifndef alledge_shader_program_h
#define alledge_shader_program_h

#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>

class Shader;

class Shader_program
{
public:
	Shader_program();
	~Shader_program();
	void Attach_shader(Shader &shader);
	void Detach_shader(Shader &shader);
	GLuint Get_glprogram();
	GLint Get_uniform_location(const char* name);
	void Link();
	void Use();
private:
	GLuint program;
};

#endif
