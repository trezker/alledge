#include "../alledge/Billboardnode.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>

void Billboardnode::Set_position(Vector3 p)
{
	position = p;
}

Vector3 Billboardnode::Get_position()
{
	return position;
}

void Billboardnode::Update_vectors(Vector3 camera_position)
{
	Vector3 front;
	Vector3 right;
	Vector3 up;
	front	= camera_position - position;
	front.y = 0;
	front.Normalize();
	// right hand rule cross products
	right	= Vector3(0, 1, 0).CrossProduct(front);
	right.Normalize();
	up		= front.CrossProduct(right);

	matrix[0] = right.x;
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
	matrix[12] = position.x;
	matrix[13] = position.y;
	matrix[14] = position.z;
	matrix[15] = 1;
}

void Billboardnode::Prerender()
{
	glPushMatrix();
	glMultMatrixf(matrix);
}

void Billboardnode::Postrender()
{
	glPopMatrix();
}
