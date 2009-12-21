#ifndef Cameranode_h
#define Cameranode_h

#include <vector>
#include "math/Vector3.h"
#include "Scenenode.h"
#include "Quat.h"

class Cameranode: public Scenenode
{
public:
	Cameranode();

	void Look_at(Vector3 p);
	Vector3 Get_up();
	Vector3 Get_front();
	Vector3 Get_right();

	void Set_position(Vector3 v);
	void Set_rotation(Vector3 v);
	Vector3 Get_position();
	Vector3 Get_rotation();
	
	void Set_rotate_around_world_origo(bool t);
	
	virtual void Prerender();
	virtual void Postrender();
private:
	Vector3 position;
	Vector3 rotation;
	bool lookat;
	Vector3 lookat_target;
	Vector3 up;
	Vector3 front;
	Vector3 right;
	quat4_t quat_total;
	
	bool rotate_around_world_origo;
};

#endif
