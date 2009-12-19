#include "../alledge/View.h"
#include <GL/glu.h>

void Init_perspective_view(float fov, float aspect, float near, float far)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(fov, aspect, near, far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Pop_view()
{
	//Return to Allegros 2D world
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
