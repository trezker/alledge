#include "../../alledge/Quat.h"

class Player {
private:
	shared_ptr<Scenenode> parent;
	shared_ptr<Static_model> model;
	shared_ptr<Static_model_node> model_node;
	shared_ptr<Transformnode> transform;

	Vector3 position;
	Vector3 velocity;
	Vector3 rotation;
	Vector3 up;
	Vector3 front;
	Vector3 right;
	quat4_t quat_total;
	bool key_up;
	bool key_down;
	bool key_left;
	bool key_right;
public:
	Player(shared_ptr<Scenenode> p);
	void Event(ALLEGRO_EVENT &event);
	void Update(float dt);
	void Set_rotation(Vector3 v);
	void Rotate_local_axis(Vector3 v);
};