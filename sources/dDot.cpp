#include "dDot.h"
#include "dBitmap.h"

#include <math.h>
#include <stdio.h>

#ifndef abs
#define abs(x) (x<0?-x:x)
#endif

dDot::dDot()
{
	num_vertices = 1;
}

dDot::~dDot()
{
}

float dDot::Z()
{
	return vex[0]->z;
}

bool dDot::IsFront()
{
	return true;
}

void dDot::Draw(dBitmap *bitmap)
{
	bitmap->AddSubPixel(vex[0]);
}

