#include "../alledge/Lightnode.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>

//Initialized to opengl default values.
Lightnode::Lightnode()
:directional(0)
{
	position.z = 1;
	
	for(int i = 0; i<3; ++i)
	{
		ambient[i] = 0;
		diffuse[i] = 0;
		specular[i] = 0;
	}
	ambient[3] = 1;
	diffuse[3] = 1;
	specular[3] = 1;
}

void Lightnode::Set_ambient(float r, float g, float b, float a)
{
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;
}

void Lightnode::Set_diffuse(float r, float g, float b, float a)
{
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;
}

void Lightnode::Set_specular(float r, float g, float b, float a)
{
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;
}

void Lightnode::Prerender()
{
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]= { position.x, position.y, position.z, directional?0.0f:1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
}

void Lightnode::Postrender()
{
	glDisable(GL_LIGHT1);
}

void Lightnode::Set_position(Vector3 pos, bool d)
{
	position = pos;
	directional = d;
}
