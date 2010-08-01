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

			//First point outside loop to set initial low/high corners
			Vector3 p;
			f>>p.x;
			f>>p.z;
			f>>p.y;
			coords.push_back(p);

			low_corner = p;
			high_corner = p;

			for(int i=1; i<num_coords; ++i)
			{
				Vector3 p;
				f>>p.x;
				f>>p.z;
				f>>p.y;
				coords.push_back(p);
				
				if(p.x < low_corner.x)
					low_corner.x = p.x;
				else if(p.x > high_corner.x)
					high_corner.x = p.x;

				if(p.y < low_corner.y)
					low_corner.y = p.y;
				else if(p.y > high_corner.y)
					high_corner.y = p.y;

				if(p.z < low_corner.z)
					low_corner.z = p.z;
				else if(p.z > high_corner.z)
					high_corner.z = p.z;
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
				faces.push_back(index);
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
	for(Indexes::iterator i=faces.begin(); i!=faces.end(); ++i, ++uv)
	{
		glNormal3f(normals[*i].x, normals[*i].y, normals[*i].z);
		glTexCoord2f(uv->u, uv->v);
		glVertex3f(coords[*i].x, coords[*i].y, coords[*i].z);
	}
	glEnd();
/*
	glBegin(GL_LINES);
	glColor4f(1, 1, 1, 1);
	for(int i = 0; i<coords.size(); ++i)
	{
		glVertex3f(coords[i].x, coords[i].y, coords[i].z);
		glVertex3f(coords[i].x + normals[i].x, coords[i].y + normals[i].y, coords[i].z + normals[i].z);
	}
	glEnd();
	*/
}

Vector3 Static_model::Get_low_corner()
{
	return low_corner;
}

Vector3 Static_model::Get_high_corner()
{
	return high_corner;
}

void Static_model::Set_model_data(Vectors c, Indexes f)
{
	coords.clear();
	faces.clear();
	normals.resize(c.size());
	uv_coords.resize(c.size());
	for(Vectors::iterator i=c.begin(); i!=c.end(); ++i)
	{
		coords.push_back(*i);
	}
	for(Indexes::iterator i=f.begin(); i!=f.end(); ++i)
	{
		faces.push_back(*i);
	}
	for(Vectors::iterator i = normals.begin(); i != normals.end(); ++i)
	{
		i->Zero();
	}
	for(int i = 0; i<faces.size(); i+=3)
	{
		Vector3 in1 = coords[faces[i+1]] - coords[faces[i]];
		Vector3 in2 = coords[faces[i+2]] - coords[faces[i]];
		Vector3 norm = in2.CrossProduct(in1);
		norm.Normalize();
		for (int j = 0; j < 3; ++j)
		{
			normals[faces[i+j]] += norm;
		}
	}
	for(Vectors::iterator i = normals.begin(); i != normals.end(); ++i)
	{
		i->Normalize();
	}
}
