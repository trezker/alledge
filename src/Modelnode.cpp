#include "../alledge/Modelnode.h"
#include <fstream>
#include "math/Vector3.h"
#include "../alledge/Quadnode.h"

void Modelnode::Set_texture(ALLEGRO_BITMAP* t)
{
	texture = t;
}

void Modelnode::Load_tmf(const std::string& filename)
{
	std::ifstream f;
	f.open(filename.c_str());
	while(!f.eof())
	{
		char t;
		f>>t;
		if(t=='C')
		{
			int num_coords;
			f>>num_coords;
			for(int i=0; i<num_coords; ++i)
			{
				Vector3 p;
				f>>p.x;
				f>>p.z;
				f>>p.y;
				coords.push_back(p);
			}
		}
		if(t=='N')
		{
			int num_normals;
			f>>num_normals;
			for(int i=0; i<num_normals; ++i)
			{
				Vector3 p;
				f>>p.x;
				f>>p.z;
				f>>p.y;
				normals.push_back(p);
			}
		}
		if(t=='F')
		{
			int num_faces;
			f>>num_faces;
			for(int i = 0; i<num_faces*3; ++i)
			{
				int index;
				f>>index;
				quads.push_back(index);
			}
		}
		if(t=='T')
		{
			int num_coords;
			f>>num_coords;
			for(int i=0; i<num_coords; ++i)
			{
				UV_coord uv_coord;
				f>>uv_coord.u;
				f>>uv_coord.v;
				uv_coords.push_back(uv_coord);
			}
		}
	}
	is_tmf = true;
	f.close();
}

void Modelnode::Loadmodel(const std::string& filename)
{
	if(filename.substr(filename.length()-4, 4) == ".tmf")
	{
		Load_tmf(filename);
		return;
	}
	is_tmf = false;
	
	std::ifstream f;
	f.open(filename.c_str());
	while(!f.eof())
	{
		Vector3 p[4];
		for(int i=0; i<4; ++i)
		{
			f>>p[i].x;
			f>>p[i].z;
			f>>p[i].y;
		}
		Quadnode* quad = new Quadnode;
		quad->Set_corners(p);
		Attach_node(quad);
	}
	f.close();
}

void Modelnode::Prerender()
{
	if(texture)
	{
		glBindTexture(GL_TEXTURE_2D, al_get_opengl_texture(texture));
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);

		glAlphaFunc(GL_GREATER,0.1f);
		glEnable(GL_ALPHA_TEST);
	}
}

void Modelnode::Render()
{
	if(is_tmf)
	{
//		glBegin(GL_QUADS);
		glBegin(GL_TRIANGLES);
		UV_coords::iterator uv=uv_coords.begin();
		for(Indexes::iterator i=quads.begin(); i!=quads.end(); ++i, ++uv)
		{
			glNormal3f(normals[*i].x, normals[*i].y, normals[*i].z);
//			glTexCoord2f(0, 0); glVertex3f(coords[*i].x, coords[*i].y, coords[*i].z);
			glTexCoord2f(uv->u, uv->v); glVertex3f(coords[*i].x, coords[*i].y, coords[*i].z);
		}
		glEnd();
	}
}

void Modelnode::Postrender()
{
	if(texture)
	{
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glShadeModel(GL_FLAT);
		glDisable(GL_ALPHA_TEST);
	}
}
