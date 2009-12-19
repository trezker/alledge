#ifndef Animated_model_instance_h
#define Animated_model_instance_h

#include "Scenenode.h"
#include <string>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <vector>
#include <map>
#include "math/Vector3.h"
#include "md5model.h"
#include "../alledge/Animated_model.h"

//Todo: Add texture here that can override the models default texture.
//Todo: All shared pointers plz

class Animated_model_instance: public Scenenode
{
public:
	Animated_model_instance();
	~Animated_model_instance();

	void Play_animation(const std::string& name, bool loop);
	void Pause_animation(bool b);
	bool Animation_has_ended();

	void Set_model(Animated_model* m);
	void Add_model(Animated_model* m);
	void Remove_model(Animated_model* m);
	void Attach_to_bone(const std::string& bone, Scenenode* node);
	void Detach_from_bone(const std::string& bone, Scenenode* node);

	void Render();
	
	void Update(double dt);
private:
	bool paused;
	anim_info_t animInfo;
	md5_anim_t *active_animation;
	md5_joint_t* allocated_skeleton;
	Animated_model* model;
	typedef std::vector<Animated_model*> Models;
	Models models;

	typedef std::vector<Scenenode*> Attachments;
	typedef std::map<std::string, Attachments> Bone_attachments;
	Bone_attachments bone_attachments;
};

#endif
