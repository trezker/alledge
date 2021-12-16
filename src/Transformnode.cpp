#include "../alledge/Transformnode.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include "../alledge/Matrix4.h"

Transformnode::Transformnode()
{
	scale.Set(1, 1, 1);
	rotate_matrix_set = false;
}

void Transformnode::Set_position(Vector3 v)
{
	position = v;
}

void Transformnode::Set_rotation(Vector3 v)
{
	rotation = v;
}

void Transformnode::Set_rotation_matrix(const float *m)
{
	for(int i = 0; i<16; ++i) {
		rotation_matrix[i] = m[i];
	}
	rotate_matrix_set = true;
}

void Transformnode::Set_scale(Vector3 v)
{
	scale = v;
}

Vector3 Transformnode::Get_position()
{
	return position;
}

Vector3 Transformnode::Get_rotation()
{
	return rotation;
}

Vector3 Transformnode::Get_scale()
{
	return scale;
}

Matrix4 Transformnode::Get_matrix()
{
	glPushMatrix();
		glLoadIdentity();
		Prerender();
		float model[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, model);
		Postrender();
	glPopMatrix();

	Matrix4 m;
	m.Set(model);
	return m;
}

void Transformnode::Prerender()
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	if(rotate_matrix_set) {
		glMultMatrixf(rotation_matrix);
	}
	else {
		glRotatef(rotation.x, 1, 0, 0);
		glRotatef(rotation.y, 0, 1, 0);
		glRotatef(rotation.z, 0, 0, 1);
	}
	glScalef(scale.x, scale.y, scale.z);
}

void Transformnode::Postrender()
{
	glPopMatrix();
}
