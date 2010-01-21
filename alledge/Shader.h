#ifndef alledge_shader_h
#define alledge_shader_h

#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <string>

class Shader
{
public:
	Shader();
	~Shader();
	bool Load_shader(const std::string& filename, GLenum shaderType);
	void Attach_to_program(GLuint program);
	void Detach_from_program(GLuint program);
private:
	GLuint shader;
};

#endif
