#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include <iostream>

#include "../../alledge/View.h"
#include "../../alledge/Scenenode.h"
#include "../../alledge/Cameranode.h"
#include "../../alledge/Lightnode.h"
#include "../../alledge/Transformnode.h"
#include "../../alledge/Static_model_node.h"
#include "../../alledge/Bitmap.h"

#include "marching_cubes.h"

Scenenode root;
shared_ptr<Cameranode> camera;
shared_ptr<Lightnode> light;
shared_ptr<Transformnode> transform;
shared_ptr<Transformnode> transform2;
shared_ptr<Static_model> model;
shared_ptr<Static_model_node> model_node;

bool Init()
{
	camera = new Cameranode();
	camera->Set_position(Vector3(0, 0, 5));
	camera->Set_rotation(Vector3(0, 0, 0));
	root.Attach_node(camera);

	light = new Lightnode;
	light->Set_position(Vector3(1, 1, 1), true);
	camera->Attach_node(light);

	transform = new Transformnode;
	light->Attach_node(transform);

	model = new Static_model;
/*	model->Load_model("data/handgun.tmf");
	shared_ptr<Bitmap> texture = new Bitmap;
	if(texture->Load("data/handgun.png"))
		model->Set_texture(texture);
*/
	Static_model::Vectors c;// = new Static_model::Vectors();
	c.push_back(Vector3(0, 0, 1));
	c.push_back(Vector3(0, 1, 0));
	c.push_back(Vector3(1, 0, 0));
	Static_model::Indexes f;// = new Static_model::Indexes();
	f.push_back(0);
	f.push_back(1);
	f.push_back(2);
//	model->Set_model_data(c, f);

	MCubesRef *mc = new MCubesRef;
	for(int x = -8; x<8; ++x) {
		for(int y = -8; y<8; ++y) {
			for(int z = -8; z<8; ++z) {
				mc->MarchCube(Vector3(x, y, z));
			}
		}
	}
	model->Set_model_data(mc->vertices, mc->indices);

	float color[4] = {1, 0, 1, 1};
//	model->Set_color(color);
	model_node = new Static_model_node;
	model_node->Set_model(model);
	transform->Attach_node(model_node);
	transform->Set_position(Vector3(0, 0, -18));

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
		transform->Set_rotation(Vector3(0, event.mouse.x, 0));
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

