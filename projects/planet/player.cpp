#include "../../alledge/shared_ptr.h"
#include "../../alledge/Scenenode.h"
#include "../../alledge/Transformnode.h"
#include "../../alledge/Static_model_node.h"
#include "player.h"

typedef std::vector<Vector3> Vectors;
typedef std::vector<int> Indexes;

Player::Player(shared_ptr<Scenenode> p) {
	key_up = false;
	key_down = false;
	key_left = false;
	key_right = false;

	front.y = 1;
	up.z = 1;

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

	position.z = 1;
	transform->Set_position(position);
	transform->Set_scale(Vector3(0.1, 0.1, 0.1));
}

void Player::Event(ALLEGRO_EVENT &event) {
	switch(event.type) {
		case ALLEGRO_EVENT_KEY_DOWN:
			if(event.keyboard.keycode == ALLEGRO_KEY_W)
				key_up = true;
			if(event.keyboard.keycode == ALLEGRO_KEY_S)
				key_down = true;
			if(event.keyboard.keycode == ALLEGRO_KEY_A)
				key_left = true;
			if(event.keyboard.keycode == ALLEGRO_KEY_D)
				key_right = true;
			break;
		case ALLEGRO_EVENT_KEY_UP:
			if(event.keyboard.keycode == ALLEGRO_KEY_W)
				key_up = false;
			if(event.keyboard.keycode == ALLEGRO_KEY_S)
				key_down = false;
			if(event.keyboard.keycode == ALLEGRO_KEY_A)
				key_left = false;
			if(event.keyboard.keycode == ALLEGRO_KEY_D)
				key_right = false;
			break;
	}
}

void Player::Update(float dt) {
	if(key_up)
		velocity = front;
	else if(key_down)
		velocity = -front;

	position += velocity * dt;
	transform->Set_position(position);
}
