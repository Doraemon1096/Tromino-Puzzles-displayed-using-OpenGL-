#include "Color.h"
#include <GL\glut.h>

Color::Color()
{
	R = G = B = 0;
}

Color Color::operator=(const Color& c)
{
	this->R = c.R;
	this->G = c.G;
	this->B = c.B;
	return (*this);
}
