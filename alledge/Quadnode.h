#ifndef Quadnode_h
#define Quadnode_h

#include "math/Vector3.h"
#include "Scenenode.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>

class Quadnode: public Scenenode
{
public:
	Quadnode();
	void Set_corners(Vector3 p[4]);
	void Set_texture(ALLEGRO_BITMAP* t);
	virtual void Render();
private:
	Vector3 v[4];
	Vector3 normal;
	ALLEGRO_BITMAP* texture;
};

#endif
