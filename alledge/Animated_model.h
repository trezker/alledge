#ifndef Animated_model_h
#define Animated_model_h

#include "Scenenode.h"
#include <string>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <vector>
#include <map>
#include "math/Vector3.h"
#include "md5model.h"
#include "Mesh.h"
class Bitmap;

typedef std::vector<Mesh> Meshbuffers;

class Animated_model
{
public:
	Animated_model();
	~Animated_model();
	void Set_texture(shared_ptr<Bitmap> t);
	shared_ptr<Bitmap> Get_texture() const;
	void Load_model(const std::string& filename);
	void Load_animation(const std::string& filename, const std::string& name);

	md5_anim_t* Get_animation(const std::string& name);
	int Get_num_joints();
	void New_buffers(Meshbuffers& buffers);

	bool Has_bone(const std::string& bone);
	
	void Set_skeleton(md5_joint_t *skeleton);

	void Render();
	void Apply_bone(const std::string& name);
	
	void Update(double dt);
private:
	typedef std::map<std::string, md5_anim_t> Animations;
	Animations animations;

	shared_ptr<Bitmap> texture;

	md5_model_t md5file;

	md5_joint_t *skeleton;

	typedef std::map<std::string, int> Bones;
	Bones bones;

	Meshbuffers meshbuffers;
};

#endif
