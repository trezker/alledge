#include "../alledge/Quadnode.h"

Quadnode::Quadnode()
:texture(NULL)
{}

void Quadnode::Set_corners(Vector3 p[4])
{
	for(int i=0; i<4; ++i)
		v[i] = p[i];

	Vector3 v1 = v[2] - v[1];
	Vector3 v2 = v[0] - v[1];
	normal = v2.CrossProduct( v1 ).GetNormalized();
}

void Quadnode::Set_texture(ALLEGRO_BITMAP* t)
{
	texture = t;
}

void Quadnode::Render()
{
	if(texture)
	{
		glBindTexture(GL_TEXTURE_2D, al_get_opengl_texture(texture));
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);

		glAlphaFunc(GL_GREATER,0.1f);
		glEnable(GL_ALPHA_TEST);
	}

	glBegin(GL_QUADS);
	glNormal3f(normal.x, normal.y, normal.z);
	glTexCoord2f(0, 0); glVertex3f(v[0].x, v[0].y, v[0].z);
	glTexCoord2f(1, 0); glVertex3f(v[1].x, v[1].y, v[1].z);
	glTexCoord2f(1, 1); glVertex3f(v[2].x, v[2].y, v[2].z);
	glTexCoord2f(0, 1); glVertex3f(v[3].x, v[3].y, v[3].z);
	glEnd();

	if(texture)
	{
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glShadeModel(GL_FLAT);
	}
}
