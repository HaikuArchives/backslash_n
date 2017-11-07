#ifndef _dPOLYGON_
#define _dPOLYGON_

#include "dVertex.h"
#include "dColor.h"
#include "dMaterial.h"

#include <OS.h>
#include <GraphicsDefs.h>

class dPolygon
{
public:
	virtual bool IsFront();
	virtual void CalcNormal();

	virtual dVertex Normal();

	virtual void CalcZ();
	virtual float Z();

	virtual void Draw(dBitmap *bitmap);

	uint8 VertexCount();

	dVertex *vex[3];		// the vertices
	uint32 u[3],v[3];		// u,v coordinates
	dMaterial *material;	// pointer to the material
protected:
	uint8 num_vertices;	// is it a dot, a line or a triangle?
};

#endif // _dPOLYGON_