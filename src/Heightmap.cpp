#include "../alledge/Heightmap.h"
#include "../alledge/Bitmap.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <cassert>
#include <iostream>

Heightmap::Heightmap()
:width(0)
,height(0)
,tilesize(0)
,num_tris(0)
,vertices(NULL)
,normals(NULL)
{
}

Heightmap::~Heightmap()
{
	if(width!=0)
	{
		delete [] vertices;
		delete [] normals;
		delete [] vertexIndices;
	}
}

void Heightmap::Set_texture(shared_ptr<Bitmap> t)
{
	texture = t;
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
	}
	width = w;
	height = h;
	vertices = new Vector3[width*height];
	normals = new Vector3[width*height];

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

//	std::cout<<"Hello"<<std::endl;
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
//	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer (3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);
//	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glDrawElements (GL_TRIANGLES, num_tris * 3, GL_UNSIGNED_INT, vertexIndices);

//	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_VERTEX_ARRAY);

	glEnd();
}

int Heightmap::Get_index(int x, int z)
{
	return x*height + z;
}
