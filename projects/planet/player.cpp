#include "../../alledge/shared_ptr.h"
#include "../../alledge/Scenenode.h"
#include "../../alledge/Transformnode.h"
#include "../../alledge/Static_model_node.h"
#include "player.h"

typedef std::vector<Vector3> Vectors;
typedef std::vector<int> Indexes;

Player::Player(shared_ptr<Scenenode> p) {
	parent = p;
	transform = new Transformnode;
	parent->Attach_node(transform);

	Vectors pv;
	Indexes pi;

	pv.push_back(Vector3(-0.5, 0, 0));
	pv.push_back(Vector3(0.5, 0, 0));
	pv.push_back(Vector3(0, 1, 0));
	pv.push_back(Vector3(0, 0, 0.5));

	pi.insert(pi.end(), {0, 2, 3, 1, 3, 2});

	model = new Static_model;
	model->Set_model_data(pv, pi);
	float color[4] = {1, 0, 1, 1};
	model->Set_color(color);

	model_node = new Static_model_node;
	model_node->Set_model(model);
	transform->Attach_node(model_node);
	transform->Set_position(Vector3(0, 0, 1));
	transform->Set_scale(Vector3(0.1,0.1,0.1));
}
