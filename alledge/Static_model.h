#ifndef Static_model_h
#define Static_model_h

#include <string>
#include <vector>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include "Vector3.h"
#include "shared_ptr.h"

class Bitmap;

struct UV_coord
{
	float u;
	float v;
};

class Static_model
{
public:
	Static_model();
	~Static_model();
	void Set_texture(shared_ptr<Bitmap> t);
	void Load_model(const std::string& filename);

	void Render();
private:
	shared_ptr<Bitmap> texture;
	typedef std::vector<Vector3> Vectors;
	Vectors coords;
	Vectors normals;
	typedef std::vector<int> Indexes;
	Indexes quads;
	typedef std::vector<UV_coord> UV_coords;
	UV_coords uv_coords;
};

#endif
