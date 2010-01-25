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

Vector3 Unproject(int x, int y, shared_ptr<Cameranode> camera)
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
