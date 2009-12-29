#ifndef Animated_model_node_h
#define Animated_model_node_h

#include "Scenenode.h"
#include <string>
#include <vector>
#include <map>

//#include "Animated_model.h"
#include "Animated_model_instance.h"
#include "shared_ptr.h"

//Todo: Add texture here that can override the models default texture.
//Todo: All shared pointers plz

class Animated_model_node: public Scenenode
{
public:
	Animated_model_node();
	~Animated_model_node();

/*	void Play_animation(const std::string& name, bool loop);
	void Pause_animation(bool b);
	bool Animation_has_ended();
*/
//	void Set_model(shared_ptr<Animated_model> m);
	void Set_model(shared_ptr<Animated_model_instance> m);
/*	void Add_model(shared_ptr<Animated_model> m);
	void Remove_model(shared_ptr<Animated_model> m);
	void Attach_to_bone(const std::string& bone, shared_ptr<Scenenode> node);
	void Detach_from_bone(const std::string& bone, shared_ptr<Scenenode> node);
*/
	void Render();
	
//	void Update(double dt);
private:
	bool paused;
	anim_info_t animInfo;
	md5_anim_t *active_animation;
	md5_joint_t* allocated_skeleton;
//	shared_ptr<Animated_model> model;
	shared_ptr<Animated_model_instance> model;
/*	typedef std::vector< shared_ptr<Animated_model> > Models;
	Models models;

	typedef std::vector< shared_ptr<Scenenode> > Attachments;
	typedef std::map<std::string, Attachments> Bone_attachments;
	Bone_attachments bone_attachments;
*/
};

#endif
