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
#include "../alledge/Bitmap.h"
#include <fstream>
#include <cmath>

#include <GL/glu.h>

Scenenode root;
shared_ptr<Cameranode> camera;
shared_ptr<Lightnode> light;
shared_ptr<Transformnode> transform;
shared_ptr<Heightmap> heightmap;
shared_ptr<Linenode> line;
shared_ptr<Bitmap> texture;
shared_ptr<Bitmap> texture2;
shared_ptr<Bitmap> splat_texture;
/*
Vector3 GetOGLPos(int x, int y, shared_ptr<Cameranode> camera)
{
	camera->Prerender();

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	camera->Postrender();

	return Vector3(posX, posY, posZ);
}
*/
bool Init()
{
	int MaxTextureImageUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureImageUnits);
	std::cout<<"Max texture units: "<<MaxTextureImageUnits<<std::endl;

	texture = new Bitmap;
	texture->Load("data/darwinian.png");
	if(!texture)
	{
		std::cout<<"ERROR: Could not load darwinian.png"<<std::endl;
		return false;
	}
	texture2 = new Bitmap;
	texture2->Load("data/Colormap.png");
	if(!texture2)
	{
		std::cout<<"ERROR: Could not load Colormap.png"<<std::endl;
		return false;
	}
	splat_texture = new Bitmap;
	splat_texture->Load("data/splat_texture.png");
	if(!splat_texture)
	{
		std::cout<<"ERROR: Could not load splat_texture.png"<<std::endl;
		return false;
	}

	camera = new Cameranode();
	camera->Set_position(Vector3(0, 5, 30));
	camera->Set_rotation(Vector3(0, 0, 0));
	root.Attach_node(camera);

	light = new Lightnode;
	light->Set_position(Vector3(0, 10, 0), false);
//	light->Set_position(Vector3(1, 1, 0), true);
/*	light->Set_ambient(2, 2, 2, 1);
	light->Set_diffuse(1, 1, 1, 1);
	light->Set_specular(1, 1, 1, 1);
*/	camera->Attach_node(light);

	transform = new Transformnode;
	light->Attach_node(transform);

	float color[4] = {1, 1, 1, 1};
	line = new Linenode;
	line->Set_line(Vector3(0, 0, 0), Vector3(0, 0, 1));
	line->Set_color(color);
	transform->Attach_node(line);

	heightmap = new Heightmap;
	heightmap->Set_tilesize(1);
	heightmap->Resize(50, 25);
	heightmap->Set_texture(texture, 0);
	heightmap->Set_texture(texture2, 1);
	heightmap->Set_splat_texture(splat_texture);
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

float fov = 45.f;
float near = 1.f;
float far = 1000.f;
float width = 640;
float height = 480;

bool lmb = false;
bool rmb = false;
int mouse_x = 0;
int mouse_y = 0;

void Update(float dt)
{
/*	Vector3 rot = transform->Get_rotation();
	rot.y += 30*dt;
	rot.x += 60*dt;
	rot.z += 10*dt;
	transform->Set_rotation(rot);
*/
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

	if(rmb)
	{
		Init_perspective_view(fov, width/height, near, far);
		Vector3 oglpoint = Unproject(mouse_x, mouse_y, camera);
		Pop_view();

//		std::cout<<"oglpoint x, y, z: "<<oglpoint.x<<", "<<oglpoint.y<<", "<<oglpoint.z<<std::endl;
		float curve[5] = {-1, -.7, 0, .3, 0};
		heightmap->Apply_brush(oglpoint.x, oglpoint.z, 10, 1*dt, curve, 5);
	}
}

void Render()
{
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
		if(event.mouse.button == 1)
		{
			lmb = true;
		}
		if(event.mouse.button == 2)
		{
			rmb = true;
		}
	}
	if (ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		if(event.mouse.button == 1)
		{
			lmb = false;
		}
		if(event.mouse.button == 2)
		{
			rmb = false;
		}
	}
	if (ALLEGRO_EVENT_MOUSE_AXES == event.type)
	{
		if(lmb)
		{
			camera->Set_rotation(camera->Get_rotation() + Vector3(-event.mouse.dy, -event.mouse.dx, 0));
		}
		mouse_x = event.mouse.x;
		mouse_y = event.mouse.y;
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
