#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "../alledge/Exception.h"
#include "../alledge/Animated_model.h"
#include "../alledge/Bitmap.h"

Animated_model::Animated_model()
{
}

Animated_model::~Animated_model()
{
	FreeModel (&md5file);
	
	for(Animations::iterator i = animations.begin(); i!= animations.end(); ++i)
	{
		FreeAnim(&i->second);
	}
}

void Animated_model::Set_texture(shared_ptr<Bitmap> t)
{
	texture = t;
}

shared_ptr<Bitmap> Animated_model::Get_texture() const
{
	return texture;
}

int Animated_model::Get_num_joints()
{
	return md5file.num_joints;
}

void Animated_model::Load_model(const std::string& filename)
{
	/* Load MD5 model file */
	if (!ReadMD5Model (filename.c_str(), &md5file))
	{
		throw(Exception("Could not load model file"));
		return;
	}
	for(int i = 0; i<md5file.num_joints; ++i)
	{
		bones[md5file.baseSkel[i].name] = i;
//		std::cout<<md5file.baseSkel[i].name<<" = "<<bones[md5file.baseSkel[i].name]<<std::endl;
	}
//	std::cout<<"Left_hand"<<" = "<<bones["Left_hand"]<<std::endl;

	for(int i = 0; i<md5file.num_meshes; ++i)
	{
		texture = new Bitmap;
		if(!texture->Load(md5file.meshes[i].shader))
		{
			throw(Exception("Could not load texture file: " + std::string(md5file.meshes[i].shader)));
			return;
		}
		
		meshbuffers.push_back(Mesh());
		meshbuffers[i].Set_up_buffers(&md5file.meshes[i], md5file.baseSkel);
	}
	return;
}

void Animated_model::New_buffers(Meshbuffers& buffers)
{
	for(int i = 0; i<md5file.num_meshes; ++i)
	{
		buffers.push_back(Mesh());
		buffers[i].Set_up_buffers(&md5file.meshes[i], md5file.baseSkel);
	}
}

void Animated_model::Load_animation(const std::string& filename, const std::string &name)
{
	/* Load MD5 animation file */
	md5_anim_t new_animation;
	if (!ReadMD5Anim (filename.c_str(), &new_animation))
	{
		FreeAnim (&new_animation);
	}
	else
	{
		animations[name] = new_animation;
	}
}

void Animated_model::Render()
{
	if(texture.get())
	{
		glBindTexture(GL_TEXTURE_2D, texture->get_opengl_texture());
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);
		glAlphaFunc(GL_GREATER,0.1f);
		glEnable(GL_ALPHA_TEST);
	}
	glPushMatrix();
	glRotatef (-90.f, 1.0f, .0f, .0f);

	GLfloat ambient[]= { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[]= { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glColor3f (1.0f, 1.0f, 1.0f);

	for(int i = 0; i<md5file.num_meshes; ++i)
	{
		meshbuffers[i].Prepare_frame(skeleton);
		meshbuffers[i].Render();
	}
	
//	Draw_model(md5file, skeleton);

	/* Draw skeleton */
/*	glDisable(GL_DEPTH_TEST);
	DrawSkeleton (skeleton, md5file.num_joints);
	glEnable(GL_DEPTH_TEST);
*/
	glPopMatrix();

	if(texture.get())
	{
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glShadeModel(GL_FLAT);
		glDisable(GL_ALPHA_TEST);
	}
}

void Animated_model::Apply_bone(const std::string& name, md5_joint_t *skel)
{
	md5_joint_t bone = skel[bones[name]];
	quat4_t &orient = bone.orient;
	glRotatef (-90.f, 1.0f, .0f, .0f);
	glTranslatef(bone.pos[0], bone.pos[1], bone.pos[2]);
	matrix4_t m4;
	Quat_to_matrix4(orient, m4);
	glMultTransposeMatrixf(m4);
	glRotatef (90.f, 1.0f, .0f, .0f);
}

void Animated_model::Apply_bone(const std::string& name)
{
	md5_joint_t bone = skeleton[bones[name]];
	quat4_t &orient = bone.orient;
	glRotatef (-90.f, 1.0f, .0f, .0f);
	glTranslatef(bone.pos[0], bone.pos[1], bone.pos[2]);
	matrix4_t m4;
	Quat_to_matrix4(orient, m4);
	glMultTransposeMatrixf(m4);
	glRotatef (90.f, 1.0f, .0f, .0f);
}

md5_anim_t* Animated_model::Get_animation(const std::string& name)
{
	if(animations.find(name) != animations.end())
	{
		return &animations[name];
	}
	else
	{
		return NULL;
	}
}

bool Animated_model::Has_bone(const std::string& bone)
{
	if(bones.find(bone) == bones.end())
		return false;
	return true;
}

void Animated_model::Set_skeleton(md5_joint_t *s)
{
	skeleton = s;
}

md5_joint_t *Animated_model::Get_bind_pose()
{
	return md5file.baseSkel;
}
