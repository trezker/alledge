#include "../alledge/Linenode.h"

Linenode::Linenode()
{}

void Linenode::Set_line(Vector3 s, Vector3 e)
{
	start = s;
	end = e;
}

void Linenode::Set_color(float *v)
{
	for(int i=0; i<4; ++i)
	{
		color[i] = v[i];
	}
}

void Linenode::Render()
{
//	glAlphaFunc(GL_GREATER,0.1f);
//	glEnable(GL_ALPHA_TEST);

	glBegin(GL_LINES);
	glVertex3f(start.x, start.y, start.z);
	glVertex3f(end.x, end.y, end.z);
	glEnd();

//	glDisable(GL_ALPHA_TEST);
}
