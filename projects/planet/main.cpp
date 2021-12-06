#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include <iostream>
#include <cmath>

#include "../../alledge/View.h"
#include "../../alledge/Scenenode.h"
#include "../../alledge/Cameranode.h"
#include "../../alledge/Lightnode.h"
#include "../../alledge/Transformnode.h"
#include "../../alledge/Static_model_node.h"
#include "../../alledge/Bitmap.h"

#include "marching_cubes.h"
#include "cubesphere.h"
#include "world.h"

Scenenode root;
shared_ptr<Cameranode> camera;
shared_ptr<Lightnode> light;
shared_ptr<Transformnode> transform;
shared_ptr<Transformnode> worldtransform;
shared_ptr<Static_model> player_model;
shared_ptr<Static_model_node> player_model_node;
shared_ptr<Transformnode> player_transform;
World* world;

bool Init()
{
	camera = new Cameranode();
	camera->Set_position(Vector3(0, 0, 5));
	camera->Set_rotation(Vector3(0, 0, 0));
	root.Attach_node(camera);

	light = new Lightnode;
	light->Set_position(Vector3(1, 1, 1), true);
	camera->Attach_node(light);

	worldtransform = new Transformnode;
	light->Attach_node(worldtransform);

	transform = new Transformnode;
	worldtransform->Attach_node(transform);

	player_transform = new Transformnode;
	worldtransform->Attach_node(player_transform);

	world = new World(transform);
	transform->Set_position(Vector3(0, 0, 0));

	Vectors pv;
	Indexes pi;

	pv.push_back(Vector3(-0.5, 0, 0));
	pv.push_back(Vector3(0.5, 0, 0));
	pv.push_back(Vector3(0, 1, 0));
	pv.push_back(Vector3(0, 0, 0.5));

	pi.insert(pi.end(), {0, 2, 3, 1, 3, 2});

	player_model = new Static_model;
	player_model->Set_model_data(pv, pi);
	float color[4] = {1, 0, 1, 1};
	player_model->Set_color(color);

	player_model_node = new Static_model_node;
	player_model_node->Set_model(player_model);
	player_transform->Attach_node(player_model_node);
	player_transform->Set_position(Vector3(0, 0, 1));
	player_transform->Set_scale(Vector3(0.1,0.1,0.1));

	return true;
}

void Update(float dt)
{
/*	Vector3 rot = transform->Get_rotation();
	rot.y += 30*dt;
	transform->Set_rotation(rot);
*/}

void Render()
{
	float fov = 45.f;
	float near = 1.f;
	float far = 1000.f;
	float width = 640;
	float height = 480;
	Init_perspective_view(fov, width/height, near, far);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);

	root.Apply();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	Pop_view();
}

void Event(ALLEGRO_EVENT event)
{
	if(ALLEGRO_EVENT_MOUSE_AXES == event.type)
	{
		worldtransform->Set_rotation(Vector3(event.mouse.y, event.mouse.x, 0));
	}
}

int main()
{
	al_init();
	al_install_mouse();
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();

	ALLEGRO_DISPLAY *display;
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
	al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 24, ALLEGRO_REQUIRE);
	display = al_create_display(800, 600);
	if(!display)
	{
		std::cout<<"Failed to create display"<<std::endl;
		return 0;
	}

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)display);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	if(!Init())
		return 0;

	double last_time = al_current_time();

	bool quit = false;
	while(1)
	{
		ALLEGRO_EVENT event;
		while (al_get_next_event(event_queue, &event))
		{
			if (ALLEGRO_EVENT_KEY_DOWN == event.type)
			{
				if (ALLEGRO_KEY_ESCAPE == event.keyboard.keycode)
				{
					quit = true;
				}
			}
			if (ALLEGRO_EVENT_DISPLAY_CLOSE == event.type)
			{
				quit = true;
			}
			Event(event);
		}
		if (quit)
			break;

		double current_time = al_current_time();
		double dt = current_time - last_time;
		last_time = current_time;
		Update(dt);

		al_clear_to_color(al_map_rgb(0, 0, 0));
		Render();
		al_flip_display();

		al_rest(0.001);
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}

