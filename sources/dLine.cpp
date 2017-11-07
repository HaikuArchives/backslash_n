#include "dLine.h"
#include "dBitmap.h"

#include <math.h>
#include <stdio.h>

#ifndef abs
#define abs(x) (x<0?-x:x)
#endif

dLine::dLine()
{
	num_vertices = 2;
	visible = true;
}

dLine::~dLine()
{
}

void dLine::CalcZ()
{
	z = (vex[0]->z + vex[1]->z) / 2.0;
}

float dLine::Z()
{
	return z;
}

bool dLine::IsFront()
{
	return visible;
}

void dLine::Draw(dBitmap *bitmap)
{
	bitmap->LineClut(vex[0],vex[1],material->clut);
}

