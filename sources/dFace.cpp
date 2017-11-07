#include "dFace.h"
#include "dBitmap.h"

#include <math.h>
#include <stdio.h>

#ifndef abs
#define abs(x) (x<0?-x:x)
#endif

dFace::dFace()
{
	num_vertices = 3;
}

dFace::~dFace()
{
}

void dFace::CalcZ()
{
	z = (vex[0]->z + vex[1]->z + vex[2]->z) / 3.0;
}

float dFace::Z()
{
	return z;
}

bool dFace::IsFront()
{
	int32 a=vex[1]->x-vex[0]->x;
	int32 b=vex[1]->y-vex[0]->y;

	if (abs(a) > abs(b))
	{
		if (a>0)	return ((vex[2]->x-vex[0]->x) * b / a + vex[0]->y < vex[2]->y);
		else		return ((vex[2]->x-vex[0]->x) * b / a + vex[0]->y > vex[2]->y);
		return true;
	}
	else
	{
		if (b>0)	return ((vex[2]->y-vex[0]->y) * a / b + vex[0]->x > vex[2]->x);
		else		return ((vex[2]->y-vex[0]->y) * a / b + vex[0]->x < vex[2]->x);
		return true;
	}
}

void dFace::CalcNormal()
{
	float u1= vex[1]->x - vex[0]->x;
	float u2= vex[1]->y - vex[0]->y;
	float u3= vex[1]->z - vex[0]->z;
	float v1= vex[2]->x - vex[0]->x;
	float v2= vex[2]->y - vex[0]->y;
	float v3= vex[2]->z - vex[0]->z;

	normal.x = u2 * v3 - u3 * v2;
	normal.y = u3 * v1 - u1 * v3;
	normal.z = u1 * v2 - u2 * v1;

	float length = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z );

	normal.x /= length;
	normal.y /= length;
	normal.z /= length;

//	printf("(%f,%f,%f)\n",normal.x,normal.y,normal.z);
}

void dFace::Draw(dBitmap *bitmap)
{
//	printf("drawing face %d\n",material->type);
	switch (material->type)
	{
		case MATERIAL_FLAT:
			bitmap->DrawFaceFlat(this,material->color);
		break;
		case MATERIAL_CLUT_FLAT:
			bitmap->DrawFaceFlatClut2(this,material->color,material->clut);
		break;
		case MATERIAL_TEXTURED:
		case MATERIAL_ENVMAPPED:
			bitmap->DrawFaceTextured(this,material->texture);
			break;
		case MATERIAL_CLUT_TEXTURED:
		case MATERIAL_CLUT_ENVMAPPED:
			bitmap->DrawFaceTexturedClut(this,material->texture,material->clut);
			break;
	}
}

