#pragma once
#include <GL\glut.h>
class Color
{
public:
	GLdouble R;
	GLdouble G;
	GLdouble B;
public:
	Color();
	Color operator=(const Color& c);
};

