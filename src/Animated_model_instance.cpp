#include <algorithm>

#include "../alledge/Animated_model_instance.h"
#include "../alledge/Quadnode.h"
#include "../alledge/Bitmap.h"

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
	model->New_buffers(meshbuffers);
}

void Animated_model_instance::Render()
{
	shared_ptr<Bitmap> texture = model->Get_texture();
	if(texture.get())
	{
		glBindTexture(GL_TEXTURE_2D, texture->get_opengl_texture());
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	glPushMatrix();
	glRotatef (-90.f, 1.0f, .0f, .0f);

	GLfloat ambient[]= { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[]= { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

	for(Meshbuffers::iterator i = meshbuffers.begin(); i != meshbuffers.end(); ++i)
	{
		i->Render();
	}

	glPopMatrix();

	if(texture.get())
	{
		glDisable(GL_TEXTURE_2D);
		glShadeModel(GL_FLAT);
		glDisable(GL_BLEND);
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
		
		for(Meshbuffers::iterator i = meshbuffers.begin(); i != meshbuffers.end(); ++i)
		{
			i->Prepare_frame(allocated_skeleton);
		}
	}
	else
	{
		/* No animation, use bind-pose skeleton */
		for(Meshbuffers::iterator i = meshbuffers.begin(); i != meshbuffers.end(); ++i)
		{
			md5_joint_t *bind_pose = model->Get_bind_pose();
			i->Prepare_frame(bind_pose);
		}
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

void Animated_model_instance::Apply_bone(const std::string& name)
{
	if(active_animation)
	{
		model->Apply_bone(name, allocated_skeleton);
	}
	else
	{
		model->Apply_bone(name, model->Get_bind_pose());
	}
}

bool Animated_model_instance::Has_bone(const std::string& bone)
{
	return model->Has_bone(bone);
}
