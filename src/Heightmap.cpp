#include "../alledge/Heightmap.h"
#include "../alledge/Bitmap.h"
#include "../alledge/Shader.h"
#include "../alledge/Shader_program.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <cassert>
#include <iostream>
#include <cstdio>

Heightmap::Heightmap()
:width(0)
,height(0)
,tilesize(0)
,num_tris(0)
,vertices(NULL)
,normals(NULL)
,vertexIndices(NULL)
,texcoords(NULL)
,shader_program(NULL)
{
	Shader vertex_shader;
	vertex_shader.Load_shader("data/shader.vert", GL_VERTEX_SHADER);

	Shader fragment_shader;
	fragment_shader.Load_shader("data/heightmap_shader.frag", GL_FRAGMENT_SHADER);

	shader_program = new Shader_program;
	shader_program->Attach_shader(vertex_shader);
	shader_program->Attach_shader(fragment_shader);
	shader_program->Link();
	shader_program->Use();
	GLint location = shader_program->Get_uniform_location("alpha"); 
	glUniform1i(location, 0);
	location = shader_program->Get_uniform_location("tex1"); 
	glUniform1i(location, 1);
	location = shader_program->Get_uniform_location("tex2"); 
	glUniform1i(location, 2);
}

Heightmap::~Heightmap()
{
	if(width!=0)
	{
		delete [] vertices;
		delete [] normals;
		delete [] vertexIndices;
		delete [] texcoords;
	}
	delete shader_program;
}

void Heightmap::Set_splat_texture(shared_ptr<Bitmap> t)
{
	splat_texture = t;
}

void Heightmap::Set_texture(shared_ptr<Bitmap> t, int channel)
{
	if(channel<0 || channel>2)
		return; //Invalid channel
	texture[channel] = t;

	char loc_name[10];
	sprintf(loc_name, "use_tex%i\0", channel);
	shader_program->Use();
	GLint location = shader_program->Get_uniform_location(loc_name); 
	if(t.get())
		glUniform1i(location, 1);
	else
		glUniform1i(location, 0);
}

void Heightmap::Load(const std::string& filename)
{
}

void Heightmap::Set_tilesize(float s)
{
	tilesize = s;
	if(width != 0)
	{
		for(int x = 0; x < width; ++x)
		{
			for(int z = 0; z < height; ++z)
			{
				Vector3& v = vertices[Get_index(x, z)];
				v.x = x * tilesize;
				v.z = z * tilesize;
			}
		}
	}
}

//Todo: preserve old heightmap
void Heightmap::Resize(int w, int h)
{
	assert(w>1 && h>1);
	if(width!=0)
	{
		delete [] vertices;
		delete [] normals;
		delete [] vertexIndices;
		delete [] texcoords;
	}
	width = w;
	height = h;
	vertices = new Vector3[width*height];
	normals = new Vector3[width*height];
	texcoords = new Texcoord[width*height];

	for(int x = 0; x < width; ++x)
	{
		for(int z = 0; z < height; ++z)
		{
			Vector3& v = vertices[Get_index(x, z)];
			v.x = x * tilesize;
			v.z = z * tilesize;
			v.y = 0;
			Vector3& n = normals[Get_index(x, z)];
			n.x = 0;
			n.z = 0;
			n.y = -1;
			Texcoord& t = texcoords[Get_index(x, z)];
			t.u = x/(width-1.0f);
			t.v = z/(height-1.0f);
		}
	}

	num_tris = (width-1)*2 * (height-1);
	vertexIndices = new int[num_tris*3];

	int i = 0;
	for(int x = 0; x < width-1; ++x)
	{
		for(int z = 0; z < height-1; ++z)
		{
			int v1 = Get_index(x, z);
			int v2 = Get_index(x, z+1);
			int v3 = Get_index(x+1, z+1);
			int v4 = Get_index(x+1, z);
			vertexIndices[i] = v1;
			vertexIndices[i+1] = v2;
			vertexIndices[i+2] = v3;
			i+=3;
			vertexIndices[i] = v1;
			vertexIndices[i+1] = v3;
			vertexIndices[i+2] = v4;
			i+=3;
		}
	}
}

void Heightmap::Render()
{
	if(width==0)
		return;

	glColor4f(1, 1, 1, 1);
	
	if(splat_texture.get())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, splat_texture->get_opengl_texture());
		glEnable(GL_TEXTURE_2D);

		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
	}
	for(int i = 0; i<4; ++i)
	{
		if(texture[i].get())
		{
			glActiveTexture(GL_TEXTURE1+i);
			glBindTexture(GL_TEXTURE_2D, texture[i]->get_opengl_texture());
			glEnable(GL_TEXTURE_2D);

			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);
		}
	}

	shader_program->Use();

	glShadeModel(GL_SMOOTH);
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glVertexPointer (3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);

	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	
	glDrawElements (GL_TRIANGLES, num_tris * 3, GL_UNSIGNED_INT, vertexIndices);

	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_VERTEX_ARRAY);
	glClientActiveTexture(GL_TEXTURE0);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glShadeModel(GL_FLAT);

	glUseProgram(0);
	

	if(splat_texture.get())
	{
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
	}

	for(int i = 0; i<4; ++i)
	{
		if(texture[i].get())
		{
			glActiveTexture(GL_TEXTURE1+i);
			glDisable(GL_TEXTURE_2D);
		}
	}
}

int Heightmap::Get_index(int x, int z)
{
	return x*height + z;
}
