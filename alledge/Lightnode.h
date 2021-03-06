#ifndef Lightnode_h
#define Lightnode_h

#include "Scenenode.h"
#include "Vector3.h"

class Lightnode: public Scenenode
{
public:
	Lightnode();
	virtual void Prerender();
	virtual void Postrender();
	void Set_position(Vector3 pos, bool directional);
	void Set_ambient(float r, float g, float b, float a);
	void Set_diffuse(float r, float g, float b, float a);
	void Set_specular(float r, float g, float b, float a);
	void Set_ambient(float *v);
	void Set_diffuse(float *v);
	void Set_specular(float *v);
	void Get_ambient(float *v);
	void Get_diffuse(float *v);
	void Get_specular(float *v);
private:
	static int lights_in_use;
	Vector3 position;
	bool directional;	

	float ambient[4];
	float diffuse[4];
	float specular[4];
};

#endif
