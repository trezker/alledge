#ifndef Transformnode_h
#define Transformnode_h

#include "Vector3.h"
#include "Scenenode.h"

class Transformnode: public Scenenode
{
public:
	Transformnode();
	void Set_position(Vector3 v);
	void Set_rotation(Vector3 v);
	void Set_scale(Vector3 v);
	void Set_rotation_matrix(const float *m);
	Vector3 Get_position();
	Vector3 Get_rotation();
	Vector3 Get_scale();
	Matrix4 Get_matrix();
	
	virtual void Prerender();
	virtual void Postrender();
private:
	bool rotate_matrix_set;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	float rotation_matrix[16];
};

#endif
