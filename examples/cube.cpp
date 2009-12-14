#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include <GL/glu.h>
#include <iostream>

#include "../alledge/Scenenode.h"
#include "../alledge/Cameranode.h"
#include "../alledge/Lightnode.h"
#include "../alledge/Quadnode.h"
#include "../alledge/Transformnode.h"

Scenenode root;
Cameranode* camera;
Lightnode* light;
Transformnode* transform;
Quadnode quads[6];
ALLEGRO_BITMAP* texture;

void Prerender_perspective_view(float fov, float aspect, float near, float far)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(fov, aspect, near, far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Postrender_perspective_view()
{
	//Return to Allegros 2D world
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool Init()
{
	texture = al_load_bitmap("data/darwinian.png");
	if(!texture)
	{
		std::cout<<"ERROR: Could not load darwinian.png"<<std::endl;
		return false;
	}
	
	camera = new Cameranode();
	camera->Set_position(Vector3(0, 0, 10));
	camera->Set_rotation(Vector3(0, 0, 0));
	root.Attach_node(camera);

	light = new Lightnode;
	light->Set_ambient(2, 2, 2, 1);
	light->Set_diffuse(1, 1, 1, 1);
	light->Set_specular(1, 1, 1, 1);
	camera->Attach_node(light);
	
	transform = new Transformnode;
	light->Attach_node(transform);

	Vector3 corners[6][4];
	corners[0][0] = Vector3(1, 1, -1);
	corners[0][1] = Vector3(-1, 1, -1);
	corners[0][2] = Vector3(-1, -1, -1);
	corners[0][3] = Vector3(1, -1, -1);
	corners[1][0] = Vector3(1, -1, 1);
	corners[1][1] = Vector3(-1, -1, 1);
	corners[1][2] = Vector3(-1, 1, 1);
	corners[1][3] = Vector3(1, 1, 1);

	corners[2][0] = Vector3(-1, 1, 1);
	corners[2][1] = Vector3(-1, -1, 1);
	corners[2][2] = Vector3(-1, -1, -1);
	corners[2][3] = Vector3(-1, 1, -1);
	corners[3][0] = Vector3(1, 1, -1);
	corners[3][1] = Vector3(1, -1, -1);
	corners[3][2] = Vector3(1, -1, 1);
	corners[3][3] = Vector3(1, 1, 1);

	corners[4][0] = Vector3(1, 1, 1);
	corners[4][1] = Vector3(-1, 1, 1);
	corners[4][2] = Vector3(-1, 1, -1);
	corners[4][3] = Vector3(1, 1, -1);
	corners[5][0] = Vector3(1, -1, -1);
	corners[5][1] = Vector3(-1, -1, -1);
	corners[5][2] = Vector3(-1, -1, 1);
	corners[5][3] = Vector3(1, -1, 1);

	for(int i = 0; i<6; ++i)
	{
		quads[i].Set_corners(corners[i]);
		quads[i].Set_texture(texture);
		transform->Attach_node(&quads[i]);
	}
	return true;
}

void Update(float dt)
{
	Vector3 rot = transform->Get_rotation();
	rot.y += 30*dt;
	rot.x += 60*dt;
	transform->Set_rotation(rot);
}

void Render()
{
	float fov = 45.f;
	float near = 1.f;
	float far = 1000.f;
	float width = 640;
	float height = 480;
	Prerender_perspective_view(fov, width/height, near, far);

	glEnable(GL_DEPTH_TEST);
 	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);

	root.Apply();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	Postrender_perspective_view();
}

void Event(ALLEGRO_EVENT event)
{
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
