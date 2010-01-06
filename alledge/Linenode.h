#ifndef Linenode_h
#define Linenode_h

#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>

#include "Vector3.h"
#include "Scenenode.h"

class Linenode: public Scenenode
{
public:
	Linenode();
	void Set_line(Vector3 start, Vector3 end);
	void Set_color(float *v);
	virtual void Render();
private:
	Vector3 start;
	Vector3 end;
	float color[4];
};

#endif
