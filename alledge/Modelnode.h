#ifndef Modelnode_h
#define Modelnode_h

#include "Scenenode.h"
#include <string>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <vector>
#include "math/Vector3.h"

struct UV_coord
{
	float u;
	float v;
};

class Modelnode: public Scenenode
{
public:
	void Set_texture(ALLEGRO_BITMAP* t);
	void Loadmodel(const std::string& filename);
	void Load_tmf(const std::string& filename);
	virtual void Prerender();
	virtual void Render();
	virtual void Postrender();
private:
	ALLEGRO_BITMAP* texture;
	typedef std::vector<Vector3> Vectors;
	Vectors coords;
	Vectors normals;
	typedef std::vector<int> Indexes;
	Indexes quads;
	typedef std::vector<UV_coord> UV_coords;
	UV_coords uv_coords;
	bool is_tmf;
};

#endif
