#include <algorithm>

#include "../alledge/Animated_model_node.h"
#include "../alledge/Quadnode.h"

Animated_model_node::Animated_model_node()
{
	model = NULL;
}

Animated_model_node::~Animated_model_node()
{
}

void Animated_model_node::Set_model(shared_ptr<Animated_model_instance> m)
{
	model = m;
}

/*
void Animated_model_node::Add_model(shared_ptr<Animated_model> m)
{
	models.push_back(m);
}

void Animated_model_node::Remove_model(shared_ptr<Animated_model> m)
{
	Models::iterator i = std::find(models.begin(), models.end(), m);
	models.erase(i);
}
*/
void Animated_model_node::Attach_to_bone(const std::string& bone, shared_ptr<Scenenode> node)
{
	if(model->Has_bone(bone))
	{
		bone_attachments[bone].push_back(node);
	}
}

void Animated_model_node::Detach_from_bone(const std::string& bone, shared_ptr<Scenenode> node)
{
	Attachments::iterator i = std::find(bone_attachments[bone].begin(), bone_attachments[bone].end(), node);
	bone_attachments[bone].erase(i);
}

void Animated_model_node::Render()
{
//	glPushMatrix();
//	glRotatef (-90.f, 1.0f, .0f, .0f);
	model->Render();
//	glPopMatrix();
/*	model->Set_skeleton(allocated_skeleton);
	model->Render();
	for(Models::iterator i = models.begin(); i!=models.end(); ++i)
	{
		(*i)->Set_skeleton(allocated_skeleton);
		(*i)->Render();
	}
*/
	for(Bone_attachments::iterator i = bone_attachments.begin(); i!=bone_attachments.end(); ++i)
	{
		glPushMatrix();
		model->Apply_bone(i->first);

		for(Attachments::iterator a = i->second.begin(); a!=i->second.end(); ++a)
		{
			(*a)->Apply();
		}
		glPopMatrix();
	}
}
