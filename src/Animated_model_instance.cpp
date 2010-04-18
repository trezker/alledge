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
	for(int i=0; i<4; ++i)
	{
		color[i] = 1;
	}
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

	glEnable(GL_COLOR_MATERIAL);
	glColor4fv(color);
	glShadeModel(GL_SMOOTH);
	glAlphaFunc(GL_GREATER,0.1f);
	glEnable(GL_ALPHA_TEST);

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

	Meshbuffers::iterator i = meshbuffers.begin();
	low_corner = i->Get_low_corner();
	high_corner = i->Get_high_corner();
	Vector3 lc;
	Vector3 hc;
	++i;
	for(; i != meshbuffers.end(); ++i)
	{
		lc = i->Get_low_corner();
		hc = i->Get_high_corner();

		if(lc.x < low_corner.x)
			low_corner.x = lc.x;
		if(hc.x > high_corner.x)
			high_corner.x = hc.x;

		if(lc.y < low_corner.y)
			low_corner.y = lc.y;
		if(hc.y > high_corner.y)
			high_corner.y = hc.y;

		if(lc.z < low_corner.z)
			low_corner.z = lc.z;
		if(hc.z > high_corner.z)
			high_corner.z = hc.z;
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

Vector3 Animated_model_instance::Get_low_corner()
{
	return low_corner;
}

Vector3 Animated_model_instance::Get_high_corner()
{
	return high_corner;
}

void Animated_model_instance::Set_color(float c[4])
{
	for(int i=0; i<4; ++i)
	{
		color[i] = c[i];
	}
}
