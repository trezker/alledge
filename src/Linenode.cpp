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
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//

	glLineWidth(2);
	glBegin(GL_LINES);
	glColor4fv(color);
	glVertex3f(start.x, start.y, start.z);
	glVertex3f(end.x, end.y, end.z);
	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}
