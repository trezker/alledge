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
	typedef std::vector<Vector3> Vectors;
	typedef std::vector<int> Indexes;

	Static_model();
	~Static_model();
	void Set_texture(shared_ptr<Bitmap> t);
	void Set_color(float c[4]);
	void Load_model(const std::string& filename);
	void Set_model_data(Vectors c, Indexes f);

	Vector3 Get_low_corner();
	Vector3 Get_high_corner();

	void Render();
private:
	float color[4];
	shared_ptr<Bitmap> texture;
	Vectors coords;
	Vectors normals;
	Indexes faces;
	typedef std::vector<UV_coord> UV_coords;
	UV_coords uv_coords;
	
	Vector3 low_corner;
	Vector3 high_corner;
};

#endif
