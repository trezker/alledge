#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include <iostream>

#include "../alledge/View.h"
#include "../alledge/Scenenode.h"
#include "../alledge/Cameranode.h"
#include "../alledge/Lightnode.h"
#include "../alledge/Heightmap.h"
#include "../alledge/Transformnode.h"
#include "../alledge/Linenode.h"
#include <cmath>

Scenenode root;
shared_ptr<Cameranode> camera;
shared_ptr<Lightnode> light;
shared_ptr<Transformnode> transform;
shared_ptr<Heightmap> heightmap;
shared_ptr<Linenode> line;

bool Init()
{
	camera = new Cameranode();
	camera->Set_position(Vector3(0, 0, 30));
	camera->Set_rotation(Vector3(0, 0, 0));
	root.Attach_node(camera);

	light = new Lightnode;
	light->Set_ambient(2, 2, 2, 1);
	light->Set_diffuse(1, 1, 1, 1);
	light->Set_specular(1, 1, 1, 1);
	camera->Attach_node(light);

	transform = new Transformnode;
	light->Attach_node(transform);

	float color[4] = {1, 1, 1, 1};
	line = new Linenode;
	line->Set_line(Vector3(0, 0, 0), Vector3(0, 0, 1));
	line->Set_color(color);
	transform->Attach_node(line);

	heightmap = new Heightmap;
	heightmap->Set_tilesize(1);
	heightmap->Resize(10, 5);
	transform->Attach_node(heightmap);
	return true;
}

bool move_forward = false;
bool move_backward = false;
bool move_left = false;
bool move_right = false;
bool move_up = false;
bool move_down = false;

float alpha = 1;

void Update(float dt)
{
	Vector3 rot = transform->Get_rotation();
	rot.y += 30*dt;
	rot.x += 60*dt;
	rot.z += 10*dt;
	transform->Set_rotation(rot);

	if(move_forward)
	{
		camera->Set_position(camera->Get_position() + camera->Get_front() * dt*10);
	}
	if(move_backward)
	{
		camera->Set_position(camera->Get_position() - camera->Get_front() * dt*10);
	}
	if(move_left)
	{
		camera->Set_position(camera->Get_position() - camera->Get_right() * dt*10);
	}
	if(move_right)
	{
		camera->Set_position(camera->Get_position() + camera->Get_right() * dt*10);
	}
	if(move_up)
	{
		camera->Set_position(camera->Get_position() + camera->Get_up() * dt*10);
	}
	if(move_down)
	{
		camera->Set_position(camera->Get_position() - camera->Get_up() * dt*10);
	}
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

bool lmb = false;

void Event(ALLEGRO_EVENT event)
{
	if (ALLEGRO_EVENT_KEY_DOWN == event.type)
	{
		if (ALLEGRO_KEY_W == event.keyboard.keycode)
		{
			move_forward = true;
		}
		if (ALLEGRO_KEY_S == event.keyboard.keycode)
		{
			move_backward = true;
		}
		if (ALLEGRO_KEY_A == event.keyboard.keycode)
		{
			move_left = true;
		}
		if (ALLEGRO_KEY_D == event.keyboard.keycode)
		{
			move_right = true;
		}
		if (ALLEGRO_KEY_R == event.keyboard.keycode)
		{
			move_up = true;
		}
		if (ALLEGRO_KEY_F == event.keyboard.keycode)
		{
			move_down = true;
		}
	}
	if (ALLEGRO_EVENT_KEY_UP == event.type)
	{
		if (ALLEGRO_KEY_W == event.keyboard.keycode)
		{
			move_forward = false;
		}
		if (ALLEGRO_KEY_S == event.keyboard.keycode)
		{
			move_backward = false;
		}
		if (ALLEGRO_KEY_A == event.keyboard.keycode)
		{
			move_left = false;
		}
		if (ALLEGRO_KEY_D == event.keyboard.keycode)
		{
			move_right = false;
		}
		if (ALLEGRO_KEY_R == event.keyboard.keycode)
		{
			move_up = false;
		}
		if (ALLEGRO_KEY_F == event.keyboard.keycode)
		{
			move_down = false;
		}
	}
	if (ALLEGRO_EVENT_MOUSE_BUTTON_DOWN == event.type)
	{
		lmb = true;
	}
	if (ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		lmb = false;
	}
	if (ALLEGRO_EVENT_MOUSE_AXES == event.type)
	{
		if(lmb)
		{
			camera->Set_rotation(camera->Get_rotation() + Vector3(-event.mouse.dy, -event.mouse.dx, 0));
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
