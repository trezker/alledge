#ifndef Static_model_node_h
#define Static_model_node_h

#include "Scenenode.h"
#include "Static_model.h"
#include "shared_ptr.h"

//Todo: Add texture here that can override the models default texture.

class Static_model_node: public Scenenode
{
public:
	Static_model_node();
	~Static_model_node();

	void Set_model(shared_ptr<Static_model> m);
	void Render();
private:
	shared_ptr<Static_model> model;
};

#endif
