#ifndef Lightnode_h
#define Lightnode_h

#include "Scenenode.h"

class Lightnode: public Scenenode
{
public:
	virtual void Prerender();
	virtual void Postrender();
private:
};

#endif
