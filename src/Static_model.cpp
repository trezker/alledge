#include "../alledge/Static_model.h"
#include <fstream>
#include "../alledge/Bitmap.h"

Static_model::Static_model()
{
	for(int i=0; i<4; ++i)
	{
		color[i] = 1;
	}
}

Static_model::~Static_model()
{
}

void Static_model::Set_color(float c[4])
{
	for(int i=0; i<4; ++i)
	{
		color[i] = c[i];
	}
}

void Static_model::Set_texture(shared_ptr<Bitmap> t)
{
	texture = t;
}

void Static_model::Load_model(const std::string& filename)
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
	f.close();
}

void Static_model::Render()
{
	if(texture.get())
	{
		glBindTexture(GL_TEXTURE_2D, texture->get_opengl_texture());
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	glEnable(GL_COLOR_MATERIAL);
	glColor4fv(color);
	glShadeModel(GL_SMOOTH);
	glAlphaFunc(GL_GREATER,0.1f);
	glEnable(GL_ALPHA_TEST);

	glBegin(GL_TRIANGLES);
	UV_coords::iterator uv=uv_coords.begin();
	for(Indexes::iterator i=quads.begin(); i!=quads.end(); ++i, ++uv)
	{
		glNormal3f(normals[*i].x, normals[*i].y, normals[*i].z);
		glTexCoord2f(uv->u, uv->v);
		glVertex3f(coords[*i].x, coords[*i].y, coords[*i].z);
	}
	glEnd();
}
