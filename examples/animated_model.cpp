#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include <iostream>


#include "../alledge/Exception.h"
#include "../alledge/View.h"
#include "../alledge/Scenenode.h"
#include "../alledge/Cameranode.h"
#include "../alledge/Lightnode.h"
#include "../alledge/Quadnode.h"
#include "../alledge/Transformnode.h"
#include "../alledge/Animated_model_node.h"
#include "../alledge/Static_model_node.h"
#include "../alledge/Bitmap.h"

Scenenode root;
shared_ptr<Cameranode> camera;
shared_ptr<Lightnode> light;
shared_ptr<Transformnode> transform;
shared_ptr<Transformnode> transform2;
shared_ptr<Animated_model> model;
//shared_ptr<Animated_model> skirt_model;
shared_ptr<Animated_model_instance> model_instance;
shared_ptr<Animated_model_node> model_node;
//shared_ptr<Animated_model_node> model_node2;
shared_ptr<Static_model> static_model;
shared_ptr<Static_model_node> static_model_node;

Vector3 scale;

bool Init()
{
	camera = new Cameranode();
	camera->Set_position(Vector3(0, 8, 30));
	camera->Set_rotation(Vector3(0, 0, 0));
	root.Attach_node(camera);


	light = new Lightnode;
/*	light->Set_ambient(1, 1, 1, 1);
	light->Set_diffuse(1, 1, 1, 1);
	light->Set_specular(1, 1, 1, 1);
*/	light->Set_position(Vector3(1, 1, 1), true);
	camera->Attach_node(light);
	
	transform = new Transformnode;
	light->Attach_node(transform);
	transform2 = new Transformnode;
	light->Attach_node(transform2);

	model = new Animated_model;
	try {
//	model->Load_model("../darw.md5mesh");
//	model->Load_animation("../darw_walk.md5anim", "walk");
	model->Load_model("data/Male.md5mesh");
//	model->Load_animation("data/Male_walk.md5anim", "walk");
//	model->Load_animation("data/Male_run.md5anim", "run");
//	skirt_model = new Animated_model;
//	skirt_model->Load_model("data/Male.md5mesh");
	}
	catch(Exception e) {
		std::cout<<e.Message()<<std::endl;
	}
	model_instance = new Animated_model_instance;
	model_instance->Set_model(model);
	float c[4] = {1, 0, 0, 1};
	model_instance->Set_color(c);
//	model_instance->Play_animation("walk", true);

	model_node = new Animated_model_node;
	model_node->Set_model(model_instance);
//	model_node->Add_model(skirt_model);
//	model_node->Play_animation("walk", true);
	transform->Attach_node(model_node);
	transform->Set_position(Vector3(0, 0, 0));
/*
	model_node2 = new Animated_model_node;
	model_node2->Set_model(model);
	model_node2->Play_animation("run", true);
	transform2->Attach_node(model_node2);
	transform2->Set_position(Vector3(-10, 0, 0));
	model_node->Attach_to_bone("Hand.L", transform2);
*/

	static_model = new Static_model;
	static_model->Load_model("data/handgun.tmf");
	shared_ptr<Bitmap> texture = new Bitmap;
	if(texture->Load("data/handgun.png"))
		static_model->Set_texture(texture);

	static_model_node = new Static_model_node;
	static_model_node->Set_model(static_model);
	model_node->Attach_to_bone("Hand.Grip.L", static_model_node);
	model_node->Attach_to_bone("Hand.Grip.R", static_model_node);

	scale.Set(1, 1, 1);

	return true;
}

void Update(float dt)
{
	Vector3 rot = transform->Get_rotation();
	rot.y += 30*dt;
	transform->Set_rotation(rot);
	transform2->Set_rotation(rot);
	model_instance->Update(dt);
//	model_node2->Update(dt);

	transform->Set_scale(scale);
}

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

bool x = false;
bool y = false;
bool z = false;

void Event(ALLEGRO_EVENT event)
{
	if (ALLEGRO_EVENT_KEY_DOWN == event.type)
	{
		if (ALLEGRO_KEY_X == event.keyboard.keycode)
		{
			x = !x;
		}
		if (ALLEGRO_KEY_Y == event.keyboard.keycode)
		{
			y = !y;
		}
		if (ALLEGRO_KEY_Z == event.keyboard.keycode)
		{
			z = !z;
		}
		if (ALLEGRO_KEY_PAD_PLUS == event.keyboard.keycode)
		{
			if(x)
				scale.x += .1;
			if(y)
				scale.y += .1;
			if(z)
				scale.z += .1;
		}
		if (ALLEGRO_KEY_PAD_MINUS == event.keyboard.keycode)
		{
			if(x)
				scale.x -= .1;
			if(y)
				scale.y -= .1;
			if(z)
				scale.z -= .1;
		}
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
