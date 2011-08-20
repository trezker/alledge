#ifndef Scenenode_h
#define Scenenode_h

#include <vector>
#include "shared_ptr.h"

class Scenenode
{
public:
	void Attach_node(shared_ptr<Scenenode> node);
	void Detach_node(shared_ptr<Scenenode> node);
	void Detach_all_nodes();
	void Apply();
	
	virtual void Prerender(){}
	virtual void Render(){}
	virtual void Postrender(){}
private:
	typedef std::vector< shared_ptr<Scenenode> > Children;
	Children children;
};

#endif
