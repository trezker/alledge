#ifndef Heightmap_h
#define Heightmap_h

#include <string>
#include <vector>
#include <list>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include "Vector3.h"
#include "shared_ptr.h"
#include "Scenenode.h"

class Shader_program;
class Bitmap;

struct Texcoord
{
	float u;
	float v;
};

typedef std::vector<Vector3*> Height_points;

class Heightmap: public Scenenode
{
public:
	Heightmap();
	~Heightmap();
	void Set_splat_texture(shared_ptr<Bitmap> t);
	void Set_texture(shared_ptr<Bitmap> t, int channel);
	void Load(const std::string& filename);
	void Set_texture_scale(float s);
	void Set_tilesize(float s);
	void Resize(int width, int height);
	int Get_size_x();
	int Get_size_z();
	int Get_tilesize();
	
	void Color_filled_circle(float x, float z, float radius, ALLEGRO_COLOR color);
//	void Set_height(float x, float y, float height);
	float Get_height(float x, float z);
	void Apply_brush(float x, float z, float brush_size, float brush_pressure, const float *brush, int brush_points);
	Height_points Get_height_points_in_circle(float x, float y, float radius) const;
	std::list<int> Get_indices_in_circle(float ix, float iy, float iradius) const;

	void Recalc_normals();

	void Render();
private:
	int Get_index(int x, int z) const;
	Vector3 Calc_normals_helper(Vector3 p, int x2, int y2, int x3, int y3);
	void Calc_normals_around(int x, int y);

	int width;
	int height;
	float tilesize;
	int num_tris;
	float texture_scale;

	shared_ptr<Bitmap> splat_texture;
	shared_ptr<Bitmap> texture[4];
	Vector3* vertices;
	Vector3* normals;
	Vector3* colors;
	int* vertexIndices;
	Texcoord* texcoords;
	
	Shader_program* shader_program;
};

#endif
