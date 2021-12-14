#include "../../alledge/shared_ptr.h"
#include "../../alledge/Scenenode.h"
#include "../../alledge/Transformnode.h"
#include "../../alledge/Static_model_node.h"
#include "player.h"
#include <cmath>

typedef std::vector<Vector3> Vectors;
typedef std::vector<int> Indexes;

Player::Player(shared_ptr<Scenenode> p) {
	key_up = false;
	key_down = false;
	key_left = false;
	key_right = false;

	front.y = 1;
	up.z = 1;
	right.x = 1;

	Quat_init(quat_total);
	//Rotate_local_axis(Vector3(90, 0, 0));

	parent = p;
	transform = new Transformnode;
	parent->Attach_node(transform);

	Vectors pv;
	Indexes pi;

	pv.push_back(Vector3(-0.5, 0, 0));
	pv.push_back(Vector3(0.5, 0, 0));
	pv.push_back(Vector3(0, 0, -1));
	pv.push_back(Vector3(0, 0.5, 0));

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
	transform->Set_rotation(rotation);
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
		velocity = -front;
	else if(key_down)
		velocity = front;
	else
		velocity = Vector3();

	if(key_left && !key_right) {
		Rotate_local_axis(Vector3(0, 1, 0));
		//velocity = -right;
	}
	if(!key_left && key_right) {
		Rotate_local_axis(Vector3(0, -1, 0));
		//velocity = right;
	}

	position += velocity * dt;
	transform->Set_position(position);
	transform->Set_rotation(rotation);
}

void Player::Set_rotation(Vector3 v)
{
	rotation = v;
	Vector3 rotrad = rotation*(M_PI/180);
	quat4_t quat_temp;
	Quat_init(quat_total);

	quat4_t quat_local;
	Quat_from_axisangle(quat_local, 1, 0, 0, rotrad.x);
	Quat_multQuat (quat_local, quat_total, quat_temp);
	Quat_copy(quat_temp, quat_total);

	Quat_from_axisangle(quat_local, 0, 1, 0, rotrad.y);
	Quat_multQuat (quat_local, quat_total, quat_temp);
	Quat_copy(quat_temp, quat_total);

	Quat_from_axisangle(quat_local, 0, 0, 1, rotrad.z);
	Quat_multQuat (quat_local, quat_total, quat_temp);
	Quat_copy(quat_temp, quat_total);

	vec3_t in;
	in[0] = 0;
	in[1] = 0;
	in[2] = 1;
	vec3_t out;
	Quat_rotatePoint (quat_total, in, out);
	front.x = out[0];
	front.y = out[1];
	front.z = out[2];

	in[0] = 0;
	in[1] = 1;
	in[2] = 0;
	Quat_rotatePoint (quat_total, in, out);
	up.x = out[0];
	up.y = out[1];
	up.z = out[2];

	front.Normalize();
	up.Normalize();
	right	= front.CrossProduct(up);
	right.Normalize();
}

void Player::Rotate_local_axis(Vector3 v) {
	Vector3 rotrad = v*(M_PI/180.0f);
	quat4_t quat_temp;

	quat4_t quat_local;
	Quat_from_axisangle(quat_local, right.x, right.y, right.z, rotrad.x);
	Quat_multQuat (quat_local, quat_total, quat_temp);
	Quat_copy(quat_temp, quat_total);

	Quat_from_axisangle(quat_local, up.x, up.y, up.z, rotrad.y);
	Quat_multQuat (quat_local, quat_total, quat_temp);
	Quat_copy(quat_temp, quat_total);

	Quat_from_axisangle(quat_local, front.x, front.y, front.z, rotrad.z);
	Quat_multQuat (quat_local, quat_total, quat_temp);
	Quat_copy(quat_temp, quat_total);

	vec3_t out;
	Quat_to_euler(quat_total, out);
	rotation.x = out[0] * (180 / M_PI);
	rotation.y = out[1] * (180 / M_PI);
	rotation.z = out[2] * (180 / M_PI);

	vec3_t in;
	in[0] = 0;
	in[1] = 0;
	in[2] = 1;
	Quat_rotatePoint (quat_total, in, out);
	front.x = out[0];
	front.y = out[1];
	front.z = out[2];

	in[0] = 0;
	in[1] = 1;
	in[2] = 0;
	Quat_rotatePoint (quat_total, in, out);
	up.x = out[0];
	up.y = out[1];
	up.z = out[2];

	front.Normalize();
	up.Normalize();
	right	= front.CrossProduct(up);
	right.Normalize();
}