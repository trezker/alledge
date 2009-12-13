#include "../alledge/Animated_modelnode.h"
#include <fstream>
#include "math/Vector3.h"
#include "../alledge/Quadnode.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

Animated_modelnode::Animated_modelnode()
{
	paused = false;
	animated = 0;
	allocated_skeleton = NULL;
}

Animated_modelnode::~Animated_modelnode()
{
	FreeModel (&md5file);
	
//	FreeAnim (&md5anim);

	if (animated && allocated_skeleton)
	{
		Destroy_skeleton(allocated_skeleton);
		allocated_skeleton = NULL;
	}
}

void Animated_modelnode::Set_texture(ALLEGRO_BITMAP* t)
{
	texture = t;
}

void Animated_modelnode::Load_model(const std::string& filename)
{
	/* Load MD5 model file */
	if (!ReadMD5Model (filename.c_str(), &md5file))
	{
		//Todo: Handle error
		return;
	}
	allocated_skeleton = Create_skeleton(md5file.num_joints);
	for(int i = 0; i<md5file.num_joints; ++i)
	{
		bones[md5file.baseSkel[i].name] = i;
//		std::cout<<md5file.baseSkel[i].name<<" = "<<bones[md5file.baseSkel[i].name]<<std::endl;
	}
//	std::cout<<"Left_hand"<<" = "<<bones["Left_hand"]<<std::endl;

	for(int i = 0; i<md5file.num_meshes; ++i)
	{
		texture = al_load_bitmap(md5file.meshes[i].shader);
	}
}

void Animated_modelnode::Load_animation(const std::string& filename, const std::string &name)
{
	/* Load MD5 animation file */
	active_animation = &animations[name];
	if (!ReadMD5Anim (filename.c_str(), active_animation))
	{
		FreeAnim (active_animation);
	}
	else
	{
		animInfo.curr_frame = 0;
		animInfo.next_frame = 1;

		animInfo.last_time = 0;
		animInfo.max_time = 1.0 / active_animation->frameRate;

		/* Allocate memory for animated skeleton */
//		skeleton = Create_skeleton(active_animation->num_joints);
		animated = 1;
	}
}

void Animated_modelnode::Prerender()
{
	if(texture)
	{
		glBindTexture(GL_TEXTURE_2D, al_get_opengl_texture(texture));
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);

		glAlphaFunc(GL_GREATER,0.1f);
		glEnable(GL_ALPHA_TEST);
	}
}

void Animated_modelnode::Render()
{
	glPushMatrix();
	glRotatef (-90.f, 1.0f, .0f, .0f);
	/* Draw skeleton */
//	DrawSkeleton (skeleton, md5file.num_joints);

	Draw_model(md5file, skeleton);

	glPopMatrix();

	for(Models::iterator i = models.begin(); i!=models.end(); ++i)
	{
//		glTranslatef(0, 0, -1);
		(*i)->Set_skeleton(skeleton);
		(*i)->Apply();
	}

	for(Bone_attachments::iterator i = bone_attachments.begin(); i!=bone_attachments.end(); ++i)
	{
		glPushMatrix();
		md5_joint_t bone = skeleton[bones[i->first]];
		quat4_t &orient = bone.orient;
		glRotatef (-90.f, 1.0f, .0f, .0f);
		glTranslatef(bone.pos[0], bone.pos[1], bone.pos[2]);
		matrix4_t m4;
		Quat_to_matrix4(orient, m4);
		glMultTransposeMatrixf(m4);
		glRotatef (90.f, 1.0f, .0f, .0f);
		for(Attachments::iterator a = i->second.begin(); a!=i->second.end(); ++a)
		{
			(*a)->Apply();
		}
		glPopMatrix();
	}
/*	md5_joint_t bone = skeleton[bones["Left_hand"]];
	quat4_t &orient = bone.orient;
	glRotatef (-90.f, 1.0f, .0f, .0f);
	glTranslatef(bone.pos[0], bone.pos[1], bone.pos[2]);
	matrix4_t m4;
	Quat_to_matrix4(orient, m4);
	glMultTransposeMatrixf(m4);
	glRotatef (90.f, 1.0f, .0f, .0f);

	glRotatef (-90.f, 1.0f, .0f, .0f);
	Draw_model(md5file, md5file.baseSkel);
*/
}

void Animated_modelnode::Postrender()
{
	if(texture)
	{
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glShadeModel(GL_FLAT);
		glDisable(GL_ALPHA_TEST);
	}
}

void Animated_modelnode::Update(double dt)
{
	if (animated && active_animation)
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
		skeleton = allocated_skeleton;
	}
	else
	{
		/* No animation, use bind-pose skeleton */
		skeleton = md5file.baseSkel;
	}
}

void Animated_modelnode::Pause_animation(bool b)
{
	paused = b;
}

void Animated_modelnode::Play_animation(const std::string& name, bool loop)
{
	if(animations.find(name) != animations.end())
	{
		active_animation = &animations[name];
		animInfo.curr_frame = 0;
		animInfo.next_frame = 1;

		animInfo.last_time = 0;
		animInfo.max_time = 1.0 / active_animation->frameRate;
		animInfo.loop = loop;
	}
	else
	{
		active_animation = NULL;
	}
}

bool Animated_modelnode::Animation_has_ended()
{
	if(active_animation)
	{
		if(animInfo.next_frame == 0)
			return true;
	}
	return false;
}

void Animated_modelnode::Add_model(Animated_modelnode* m)
{
	models.push_back(m);
}

void Animated_modelnode::Remove_model(Animated_modelnode* m)
{
	Models::iterator i = std::find(models.begin(), models.end(), m);
	models.erase(i);
}

void Animated_modelnode::Attach_to_bone(const std::string& bone, Scenenode* node)
{
	if(bones.find(bone) == bones.end())
	{
		std::cout<<"Attach_to_bone '"<<bone<<"': Bone does not exist"<<std::endl;
	}
	bone_attachments[bone].push_back(node);
}

void Animated_modelnode::Detach_from_bone(const std::string& bone, Scenenode* node)
{
	Attachments::iterator i = std::find(bone_attachments[bone].begin(), bone_attachments[bone].end(), node);
	bone_attachments[bone].erase(i);
}

void Animated_modelnode::Set_skeleton(md5_joint_t *s)
{
	skeleton = s;
}
