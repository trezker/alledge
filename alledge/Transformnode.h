#ifndef Transformnode_h
#define Transformnode_h

#include "math/Vector3.h"
#include "Scenenode.h"

class Transformnode: public Scenenode
{
public:
	void Set_position(Vector3 v);
	void Set_rotation(Vector3 v);
	Vector3 Get_position();
	Vector3 Get_rotation();
	
	virtual void Prerender();
	virtual void Postrender();
private:
	Vector3 position;
	Vector3 rotation;
};

#endif
