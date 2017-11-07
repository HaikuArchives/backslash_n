#ifndef _dDOT_
#define _dDOT_

#include "dVertex.h"
#include "dColor.h"
#include "dMaterial.h"
#include "dPolygon.h"

#include <OS.h>
#include <GraphicsDefs.h>

class dDot : public dPolygon
{
public:
	dDot();
	virtual ~dDot();
	virtual bool IsFront();

	virtual float Z();
	
	virtual void Draw(dBitmap *bitmap);
};

#endif // _dDOT_