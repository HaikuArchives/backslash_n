#include "dPolygon.h"

bool dPolygon::IsFront()
{
	return true;
}

void dPolygon::CalcNormal()
{
}

dVertex dPolygon::Normal()
{
	return dVertex(0.0,0.0,0.0);
}

void dPolygon::CalcZ()
{
}

float dPolygon::Z()
{
	return 0.0;
}

void dPolygon::Draw(dBitmap *bitmap)
{
}

uint8 dPolygon::VertexCount()
{
	return num_vertices;
}
