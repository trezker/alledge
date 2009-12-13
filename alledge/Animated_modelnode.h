#ifndef Animated_modelnode_h
#define Animated_modelnode_h

#include "Scenenode.h"
#include <string>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <vector>
#include <map>
#include "math/Vector3.h"
#include "md5model.h"

class Animated_modelnode: public Scenenode
{
public:
	Animated_modelnode();
	~Animated_modelnode();
	void Set_texture(ALLEGRO_BITMAP* t);
	void Load_model(const std::string& filename);
	void Load_animation(const std::string& filename, const std::string& name);

	void Play_animation(const std::string& name, bool loop);
	void Pause_animation(bool b);
	bool Animation_has_ended();

	void Add_model(Animated_modelnode* m);
	void Remove_model(Animated_modelnode* m);

	void Attach_to_bone(const std::string& bone, Scenenode* node);
	void Detach_from_bone(const std::string& bone, Scenenode* node);
	
	void Set_skeleton(md5_joint_t *skeleton);

	virtual void Prerender();
	virtual void Render();
	virtual void Postrender();
	
	void Update(double dt);
private:
	typedef std::map<std::string, md5_anim_t> Animations;
	Animations animations;
	md5_anim_t *active_animation;

	ALLEGRO_BITMAP* texture;

	md5_model_t md5file;

	int animated;
	bool paused;

	md5_joint_t *allocated_skeleton;
	md5_joint_t *skeleton;
	anim_info_t animInfo;

	typedef std::map<std::string, int> Bones;
	Bones bones;

	//Todo: Make separate Animated_model class, so you can save memory by reusing the same model
	typedef std::vector<Animated_modelnode*> Models;
	Models models;
	
	typedef std::vector<Scenenode*> Attachments;
	typedef std::map<std::string, Attachments> Bone_attachments;
	Bone_attachments bone_attachments;
};

#endif
