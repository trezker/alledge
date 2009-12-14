#include "../alledge/Lightnode.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <iostream>

int Lightnode::lights_in_use = 0;
int max_used_lights = 0;

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

void Lightnode::Set_ambient(float *v)
{
	for(int i=0; i<4; ++i)
	{
		ambient[i] = v[i];
	}
}

void Lightnode::Set_diffuse(float *v)
{
	for(int i=0; i<4; ++i)
	{
		diffuse[i] = v[i];
	}
}

void Lightnode::Set_specular(float *v)
{
	for(int i=0; i<4; ++i)
	{
		specular[i] = v[i];
	}
}

void Lightnode::Get_ambient(float *v)
{
	for(int i=0; i<4; ++i)
	{
		v[i] = ambient[i];
	}
}

void Lightnode::Get_diffuse(float *v)
{
	for(int i=0; i<4; ++i)
	{
		v[i] = diffuse[i];
	}
}

void Lightnode::Get_specular(float *v)
{
	for(int i=0; i<4; ++i)
	{
		v[i] = specular[i];
	}
}

void Lightnode::Prerender()
{
	if(lights_in_use >= GL_MAX_LIGHTS)
	{
		++lights_in_use;
		max_used_lights = lights_in_use;
		std::cout<<"Lights used are more than GL_MAX_LIGHTS by "<<max_used_lights - GL_MAX_LIGHTS<<std::endl;
		return;
	}
	int light = GL_LIGHT0+lights_in_use;
/*	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };
*/	GLfloat LightPosition[]= { position.x, position.y, position.z, directional?0.0f:1.0f };
	glLightfv(light, GL_AMBIENT, ambient);
	glLightfv(light, GL_DIFFUSE, diffuse);
	glLightfv(light, GL_SPECULAR, specular);
	glLightfv(light, GL_POSITION, LightPosition);
	glEnable(light);
	++lights_in_use;
}

void Lightnode::Postrender()
{
	if(lights_in_use > GL_MAX_LIGHTS)
	{
		--lights_in_use;
		return;
	}
	--lights_in_use;
	int light = GL_LIGHT0+lights_in_use;
	glDisable(light);
}

void Lightnode::Set_position(Vector3 pos, bool d)
{
	position = pos;
	directional = d;
}
