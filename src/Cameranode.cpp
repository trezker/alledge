#include "../alledge/Cameranode.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdio>

Cameranode::Cameranode()
:lookat(false)
,rotate_around_world_origo(false)
{}

void Cameranode::Look_at(Vector3 p)
{
	lookat_target = p;
	front	= p - position;
	front.Normalize();
	// right hand rule cross products
	right	= Vector3(0, 1, 0).CrossProduct(front);
	right.Normalize();
	up		= front.CrossProduct(right);

	lookat = true;
/*	matrix[0] = right.x;
	matrix[1] = right.y;
	matrix[2] = right.z;
	matrix[3] = 0;
	matrix[4] = up.x;
	matrix[5] = up.y;
	matrix[6] = up.z;
	matrix[7] = 0;
	matrix[8] = front.x;
	matrix[9] = front.y;
	matrix[10] = front.z;
	matrix[11] = 0;
	matrix[12] = -position.x;
	matrix[13] = -position.y;
	matrix[14] = -position.z;
	matrix[15] = 1;
*/
}

Vector3 Cameranode::Get_up()
{
	return up;
}

Vector3 Cameranode::Get_front()
{
	return front;
}

Vector3 Cameranode::Get_right()
{
	return right;
}

void Cameranode::Set_position(Vector3 v)
{
	position = v;
}

void Cameranode::Set_rotation(Vector3 v)
{
	rotation = v;

	Vector3 rotrad = rotation*(M_PI/180);
	front.x = -sin(rotrad.y);
	front.z = cos(rotrad.y);
	front.y = 0;
	front.Normalize();
	right	= Vector3(0, 1, 0).CrossProduct(front);
	right.Normalize();
	up		= front.CrossProduct(right);
}

Vector3 Cameranode::Get_position()
{
	return position;
}

Vector3 Cameranode::Get_rotation()
{
	return rotation;
}

void Cameranode::Set_rotate_around_world_origo(bool t)
{
	rotate_around_world_origo = t;
}

void Cameranode::Prerender()
{
	glPushMatrix();
	if(!lookat)
	{
		if(!rotate_around_world_origo)
		{
			glRotatef(rotation.x, 1, 0, 0);
			glRotatef(rotation.y, 0, 1, 0);
			glRotatef(rotation.z, 0, 0, 1);
			glTranslatef(-position.x, -position.y, -position.z);
		}
		else
		{
			glTranslatef(-position.x, -position.y, -position.z);
			glRotatef(rotation.x, 1, 0, 0);
			glRotatef(rotation.y, 0, 1, 0);
			glRotatef(rotation.z, 0, 0, 1);
		}
	}
	else
	{
		gluLookAt( position.x, position.y, position.z,
				   lookat_target.x, lookat_target.y, lookat_target.z,
				   up.x, up.y, up.z );
	}
}

void Cameranode::Postrender()
{
	glPopMatrix();
}
