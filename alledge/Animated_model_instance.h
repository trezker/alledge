#ifndef Animated_model_instance_h
#define Animated_model_instance_h

#include "Scenenode.h"
#include <string>
#include <vector>
#include <map>

#include "Animated_model.h"
#include "shared_ptr.h"

class Animated_model_instance: public Scenenode
{
public:
	Animated_model_instance();
	~Animated_model_instance();

	void Play_animation(const std::string& name, bool loop);
	void Pause_animation(bool b);
	bool Animation_has_ended();

	void Set_model(shared_ptr<Animated_model> m);

	bool Has_bone(const std::string& bone);
	void Apply_bone(const std::string& name);

	void Render();
	
	void Update(double dt);

	Vector3 Get_low_corner();
	Vector3 Get_high_corner();
	void Set_color(float c[4]);

private:
	float color[4];
	bool paused;
	anim_info_t animInfo;
	md5_anim_t *active_animation;
	md5_joint_t* allocated_skeleton;
	shared_ptr<Animated_model> model;

	Meshbuffers meshbuffers;

	Vector3 low_corner;
	Vector3 high_corner;
};

#endif
