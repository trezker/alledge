#include <algorithm>

#include "../alledge/Animated_model_instance.h"
#include "../alledge/Quadnode.h"

Animated_model_instance::Animated_model_instance()
{
	paused = false;
	model = NULL;
	active_animation = NULL;
	allocated_skeleton = NULL;
}

Animated_model_instance::~Animated_model_instance()
{
	if (allocated_skeleton)
	{
		Destroy_skeleton(allocated_skeleton);
		allocated_skeleton = NULL;
	}
}

void Animated_model_instance::Set_model(shared_ptr<Animated_model> m)
{
	model = m;
	if (allocated_skeleton)
	{
		Destroy_skeleton(allocated_skeleton);
	}
	int num_joints = model->Get_num_joints();
	allocated_skeleton = Create_skeleton(num_joints);
}

void Animated_model_instance::Add_model(shared_ptr<Animated_model> m)
{
	models.push_back(m);
}

void Animated_model_instance::Remove_model(shared_ptr<Animated_model> m)
{
	Models::iterator i = std::find(models.begin(), models.end(), m);
	models.erase(i);
}

void Animated_model_instance::Attach_to_bone(const std::string& bone, shared_ptr<Scenenode> node)
{
	if(model->Has_bone(bone))
	{
		bone_attachments[bone].push_back(node);
	}
}

void Animated_model_instance::Detach_from_bone(const std::string& bone, shared_ptr<Scenenode> node)
{
	Attachments::iterator i = std::find(bone_attachments[bone].begin(), bone_attachments[bone].end(), node);
	bone_attachments[bone].erase(i);
}

void Animated_model_instance::Render()
{
	model->Set_skeleton(allocated_skeleton);
	model->Render();
	for(Models::iterator i = models.begin(); i!=models.end(); ++i)
	{
		(*i)->Set_skeleton(allocated_skeleton);
		(*i)->Render();
	}

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

void Animated_model_instance::Update(double dt)
{
	if (active_animation)
	{
		/* Calculate current and next frames */
		if(!paused)
		{
			Animate (active_animation, &animInfo, dt);
		}

		/* Interpolate skeletons between two frames */
		InterpolateSkeletons (active_animation->skelFrames[animInfo.curr_frame],
			active_animation->skelFrames[animInfo.next_frame],
			active_animation->num_joints,
			animInfo.last_time * active_animation->frameRate,
			allocated_skeleton);
	}
	else
	{
		/* No animation, use bind-pose skeleton */
	}
}

void Animated_model_instance::Pause_animation(bool b)
{
	paused = b;
}

void Animated_model_instance::Play_animation(const std::string& name, bool loop)
{
	active_animation = model->Get_animation(name);
	if(active_animation)
	{
		animInfo.curr_frame = 0;
		animInfo.next_frame = 1;

		animInfo.last_time = 0;
		animInfo.max_time = 1.0 / active_animation->frameRate;
		animInfo.loop = loop;
	}
}

bool Animated_model_instance::Animation_has_ended()
{
	if(active_animation)
	{
		if(animInfo.next_frame == 0)
			return true;
	}
	return false;
}
