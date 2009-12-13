#include "../alledge/Transformnode.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>


void Transformnode::Set_position(Vector3 v)
{
	position = v;
}

void Transformnode::Set_rotation(Vector3 v)
{
	rotation = v;
}

Vector3 Transformnode::Get_position()
{
	return position;
}

Vector3 Transformnode::Get_rotation()
{
	return rotation;
}

void Transformnode::Prerender()
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glRotatef(rotation.x, 1, 0, 0);
	glRotatef(rotation.y, 0, 1, 0);
	glRotatef(rotation.z, 0, 0, 1);
}

void Transformnode::Postrender()
{
	glPopMatrix();
}
