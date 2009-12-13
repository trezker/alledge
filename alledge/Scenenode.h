#ifndef Scenenode_h
#define Scenenode_h

#include <vector>

class Scenenode
{
public:
	void Attach_node(Scenenode* node);
	void Detach_node(Scenenode* node);
	void Apply();
	
	virtual void Prerender(){}
	virtual void Render(){}
	virtual void Postrender(){}
private:
	typedef std::vector<Scenenode*> Children;
	Children children;
};

#endif
