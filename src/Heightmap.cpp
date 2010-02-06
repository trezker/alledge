#include "../alledge/Heightmap.h"
#include "../alledge/Bitmap.h"
#include "../alledge/Shader.h"
#include "../alledge/Shader_program.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <cmath>

Heightmap::Heightmap()
:width(0)
,height(0)
,tilesize(0)
,num_tris(0)
,texture_scale(1)
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
	location = shader_program->Get_uniform_location("tex3"); 
	glUniform1i(location, 3);
	location = shader_program->Get_uniform_location("tex4"); 
	glUniform1i(location, 4);
	location = shader_program->Get_uniform_location("tex_scale_s");
	glUniform1f(location, 1);
	location = shader_program->Get_uniform_location("tex_scale_t");
	glUniform1f(location, 1);
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
	if(channel<0 || channel>3)
		return; //Invalid channel
	texture[channel] = t;

	char loc_name[10];
	sprintf(loc_name, "use_tex%i\0", channel+1);
	shader_program->Use();
	GLint location = shader_program->Get_uniform_location(loc_name); 
	if(t.get())
		glUniform1i(location, 1);
	else
		glUniform1i(location, 0);
	glUseProgram(0);
}

void Heightmap::Load(const std::string& filename)
{
}

void Heightmap::Set_texture_scale(float s)
{
	if(s<=0)
		return;
	texture_scale = s;
	GLint location = shader_program->Get_uniform_location("tex_scale_s");
	glUniform1f(location, width*texture_scale);
	location = shader_program->Get_uniform_location("tex_scale_t");
	glUniform1f(location, height*texture_scale);
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

	GLint location = shader_program->Get_uniform_location("tex_scale_s");
	glUniform1f(location, width*texture_scale);
	location = shader_program->Get_uniform_location("tex_scale_t");
	glUniform1f(location, height*texture_scale);


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
			n.y = 1;
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

int Heightmap::Get_size_x()
{
	return width;
}

int Heightmap::Get_size_z()
{
	return height;
}

int Heightmap::Get_tilesize()
{
	return tilesize;
}

void Heightmap::Render()
{

	if(width==0)
		return;

	if(splat_texture.get())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, splat_texture->get_opengl_texture());
		glEnable(GL_TEXTURE_2D);
	}
	for(int i = 0; i<4; ++i)
	{
		if(texture[i].get())
		{
			glActiveTexture(GL_TEXTURE1+i);
			glBindTexture(GL_TEXTURE_2D, texture[i]->get_opengl_texture());
			glEnable(GL_TEXTURE_2D);
		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	shader_program->Use();

	glDisableClientState(GL_COLOR_ARRAY);

	GLfloat ambient[]= { 0.5f, 0.5f, 0.5f, 0.5f };
	GLfloat diffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

	glShadeModel(GL_SMOOTH);
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glVertexPointer (3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);

	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	
	glDrawElements (GL_TRIANGLES, num_tris * 3, GL_UNSIGNED_INT, vertexIndices);

	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glShadeModel(GL_FLAT);

	glUseProgram(0);
	
	for(int i = 0; i<4; ++i)
	{
		if(texture[i].get())
		{
			glActiveTexture(GL_TEXTURE1+i);
			glDisable(GL_TEXTURE_2D);
		}
	}

	if(splat_texture.get())
	{
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
	}

/*
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(2);
	glBegin(GL_LINES);
	glColor4f(1, 1, 1, 1);

	for(int x = 0; x<width; ++x)
	{
		for(int z = 0; z<height; ++z)
		{
			Vector3 start = vertices[Get_index(x, z)];
			Vector3 end = start + normals[Get_index(x, z)];
			glVertex3f(start.x, start.y, start.z);
			glVertex3f(end.x, end.y, end.z);
		}
	}

	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
*/
}

int Heightmap::Get_index(int x, int z) const
{
	return x*height + z;
}


void Heightmap::Apply_brush(float x, float z, float brush_size, float brush_pressure, const float *brush, int brush_points)
{
	float brush_scale = brush_points/brush_size;
	Height_points height_points = Get_height_points_in_circle(x, z, brush_size);
	for(Height_points::iterator hp=height_points.begin(); hp!=height_points.end(); ++hp)
	{
		Vector3* height_point = *hp;
		float dx = height_point->x-x;
		float dz = height_point->z-z;
		float r = sqrt(dx*dx + dz*dz)*brush_scale;
		int low = r;
		float p = r-low;
		if(low+1<brush_points)
		{
			float h = height_point->y+brush_pressure*((brush[low+1]-brush[low])*p+brush[low]);
			height_point->y = h;
//			Set_height(height_point->x, height_point->z, h);
		}
	}
	Recalc_normals();
}

void Heightmap::Recalc_normals()
{
	for(unsigned int ix = 0; ix<width; ++ix)
	{
		for(unsigned int iz = 0; iz<height; ++iz)
		{
//			if(rows[ix][iz].normal_dirty)
			{
				Calc_normals_around(ix, iz);
			}
		}
	}
}

Vector3 Heightmap::Calc_normals_helper(Vector3 p, int x2, int z2, int x3, int z3)
{
	if(x2<0 || z2<0 || x2>=width || z2>=height)
		return Vector3();
	if(x3<0 || z3<0 || x3>=width || z3>=height)
		return Vector3();

	Vector3 v1 = vertices[Get_index(x2, z2)] - p;
	Vector3 v2 = vertices[Get_index(x3, z3)] - p;
	return v2.CrossProduct( v1 );
}

//Calc the normal for given point
void Heightmap::Calc_normals_around(int x, int z)
{
	Vector3 sum;
	Vector3 p = vertices[Get_index(x, z)];
	sum += Calc_normals_helper(p, x+1, z, x+1, z+1);
	sum += Calc_normals_helper(p, x+1, z+1, x, z+1);
	sum += Calc_normals_helper(p, x, z+1, x-1, z+1);
	sum += Calc_normals_helper(p, x-1, z+1, x-1, z);
	sum += Calc_normals_helper(p, x-1, z, x-1, z-1);
	sum += Calc_normals_helper(p, x-1, z-1, x, z-1);
	sum += Calc_normals_helper(p, x, z-1, x+1, z-1);
	sum += Calc_normals_helper(p, x+1, z-1, x+1, z);
	sum.Normalize();
	normals[Get_index(x, z)] = sum;
}
/*
void Heightmap::Set_height(float ix, float iz, float height)
{
	int x = ix/tilesize+.5;
	int z = iz/tilesize+.5;
	if(x<0 || z<0 || x>=width || z>=height)
		return;
	vertices[Get_index(x, z)].y = height;
//	rows[x][y].normal_dirty = true;
}
*/
float Heightmap::Get_height(float ix, float iz)
{
	int tx = ix/tilesize;
	int tz = iz/tilesize;
	if(tx<0 || tz<0 || tx+1>=width || tz+1>=height)
		return 0;

	Vector3& x1z1 = vertices[Get_index(tx, tz)];
	Vector3& x2z1 = vertices[Get_index(tx+1, tz)];
	Vector3& x1z2 = vertices[Get_index(tx, tz+1)];
	Vector3& x2z2 = vertices[Get_index(tx+1, tz+1)];

	float xp = (ix-tx)/tilesize;
	float zp = (iz-tz)/tilesize;
	float hx1 = (x2z1.y-x1z1.y)*xp+x1z1.y;
	float hx2 = (x2z2.y-x1z2.y)*xp+x1z2.y;
	return (hx2-hx1)*zp+hx1;
}

Height_points Heightmap::Get_height_points_in_circle(float ix, float iy, float iradius) const
{
	int radius = iradius/tilesize+1;
	int cx = ix/tilesize+.5;
	int cy = iy/tilesize+.5;
	int sx = (cx-radius)<0?0:cx-radius;
	int sy = (cy-radius)<0?0:cy-radius;
	int ex = (cx+radius)>=width?width:cx+radius+1;
	int ey = (cy+radius)>=height?height:cy+radius+1;
	float radius_squared = iradius*iradius;

	Height_points height_points;
	for(int x = sx; x<ex; ++x)
	{
		for(int y = sy; y<ey; ++y)
		{
			float rx = x*tilesize;
			float ry = y*tilesize;
			float dx = rx-ix;
			float dy = ry-iy;
			if(dx*dx + dy*dy <= radius_squared)
			{
				Vector3* height_point = &vertices[Get_index(x, y)];
				height_points.push_back(height_point);
			}
		}
	}
	return height_points;
}
