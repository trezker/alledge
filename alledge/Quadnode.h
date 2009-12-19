#ifndef Quadnode_h
#define Quadnode_h

#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>

#include "Vector3.h"
#include "Scenenode.h"
class Bitmap;

class Quadnode: public Scenenode
{
public:
	Quadnode();
	void Set_corners(Vector3 p[4]);
	void Set_texture(shared_ptr<Bitmap> t);
	virtual void Render();
private:
	Vector3 v[4];
	Vector3 normal;
	shared_ptr<Bitmap> texture;
};

#endif
