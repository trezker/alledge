#include "../alledge/Cameranode.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdio>

#include "../alledge/Quat.h"

Cameranode::Cameranode()
:lookat(false)
,rotate_around_world_origo(false)
{
	Quat_init(quat_total);
}

void Cameranode::Look_at(Vector3 p)
{
	lookat_target = p;
	front	= p - position; //Todo: I think this is wrong, which also makes right wrong
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
	in[2] = -1;
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


/* TODO: Extract world rotation values after rotation
*/
void Cameranode::Rotate_local_axis(Vector3 v) {
	Vector3 rotrad = v*(M_PI/180);
	quat4_t quat_temp;
	Quat_init(quat_total);

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
	rotation.x -= out[0] * (180 / M_PI);
	rotation.y -= out[1] * (180 / M_PI);
	rotation.z -= out[2] * (180 / M_PI);

	//std::cout<<rotation.x<<", "<<rotation.y<<", "<<rotation.z<<std::endl;

	vec3_t in;
	in[0] = front.x;
	in[1] = front.y;
	in[2] = front.z;
	Quat_rotatePoint (quat_total, in, out);
	front.x = out[0];
	front.y = out[1];
	front.z = out[2];

	in[0] = up.x;
	in[1] = up.y;
	in[2] = up.z;
	Quat_rotatePoint (quat_total, in, out);
	up.x = out[0];
	up.y = out[1];
	up.z = out[2];

	front.Normalize();
	up.Normalize();
	right	= front.CrossProduct(up);
	right.Normalize();
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
			matrix4_t matrix;
			Quat_to_matrix4 (quat_total, matrix);

			glMultMatrixf(matrix);

/*			glRotatef(rotation.x, 1, 0, 0);
			glRotatef(rotation.y, 0, 1, 0);
			glRotatef(rotation.z, 0, 0, 1);
*/			glTranslatef(-position.x, -position.y, -position.z);
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
