#include "../alledge/Scenenode.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <algorithm>

void Scenenode::Attach_node(shared_ptr<Scenenode> node)
{
	children.push_back(node);
}

void Scenenode::Detach_node(shared_ptr<Scenenode> node)
{
	children.erase(std::find(children.begin(), children.end(), node));
}

void Scenenode::Detach_all_nodes()
{
	children.clear();
}

void Scenenode::Apply()
{
	Prerender();
	Render();
	for(Children::iterator i = children.begin(); i!=children.end(); ++i)
	{
		(*i)->Apply();
	}
	Postrender();
}
