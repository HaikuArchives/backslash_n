#include "dVertex.h"

#include <stdio.h>

_EXPORT void dVertex::Rotate(float *t,dVertex *vec)
{
	float tx = x;
	float ty = y;
	float tz = z;
	vec->x = t[0] * tx + t[1] * ty + t[2] * tz + t[3];
	vec->y = t[4] * tx + t[5] * ty + t[6] * tz + t[7];
	vec->z = t[8] * tx + t[9] * ty + t[10] * tz + t[11];
}

_EXPORT void dVertex::RotateSimple(float *t,dVertex *vec)
{
	float tx = x;
	float ty = y;
	float tz = z;
	vec->x = t[0] * tx + t[1] * ty + t[2] * tz;
	vec->y = t[4] * tx + t[5] * ty + t[6] * tz;
	vec->z = t[8] * tx + t[9] * ty + t[10] * tz;
}

_EXPORT void dVertex::Rotate3x3(float *t,dVertex *vec)
{
	float tx = x;
	float ty = y;
	float tz = z;
	vec->x = t[0] * tx + t[1] * ty + t[2] * tz;
	vec->y = t[3] * tx + t[4] * ty + t[5] * tz;
	vec->z = t[6] * tx + t[7] * ty + t[8] * tz;
}

_EXPORT uint16 dVertex::CalcZoom(uint8 fov,uint16 screen_x,uint16 screen_y,float *vert_angle,float *horiz_angle)
{
	if (fov>179) fov=179;
	*horiz_angle=fov*3.1415927/360;
	uint16 zoom=(uint16)(screen_x*cos(*horiz_angle)/sin(*horiz_angle));
	*vert_angle=atan(((float)screen_y)/((float)zoom));

	return zoom;
}
