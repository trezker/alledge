#ifndef Heightmap_h
#define Heightmap_h

#include <string>
#include <vector>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include "Vector3.h"
#include "shared_ptr.h"
#include "Scenenode.h"

class Bitmap;

struct UV_coord
{
	float u;
	float v;
};

class Heightmap: public Scenenode
{
public:
	Heightmap();
	~Heightmap();
	void Set_texture(shared_ptr<Bitmap> t);
	void Load(const std::string& filename);
	void Set_tilesize(float s);
	void Resize(int width, int height);

	void Render();
private:
	int Get_index(int x, int z);

	int width;
	int height;
	float tilesize;
	int num_tris;

	shared_ptr<Bitmap> texture;
	Vector3* vertices;
	Vector3* normals;
	int* vertexIndices;
};

#endif
