#include "../../alledge/Quat.h"
class World;

class Player {
private:
	shared_ptr<Scenenode> parent;
	shared_ptr<Static_model> model;
	shared_ptr<Static_model_node> model_node;
	shared_ptr<Transformnode> transform;

	World *world;

	Vector3 position;
	Vector3 velocity;
	//Vector3 rotation;
	Vector3 up;
	Vector3 front;
	Vector3 right;
	quat4_t quat_total;
	bool key_up;
	bool key_down;
	bool key_left;
	bool key_right;

	void Execute_rotation();
	void Rotate_on_axis(Vector3 axis, float a);
public:
	Player(shared_ptr<Scenenode> p, World *w);
	void Event(ALLEGRO_EVENT &event);
	void Update(float dt);
	void Set_rotation(Vector3 v);
	void Rotate_local_axis(Vector3 v);
	Vector3 Position();
};