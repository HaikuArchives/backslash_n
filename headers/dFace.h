#ifndef _dFACE_
#define _dFACE_

#include "dVertex.h"
#include "dColor.h"
#include "dMaterial.h"
#include "dPolygon.h"

#include <OS.h>
#include <GraphicsDefs.h>

class dFace : public dPolygon
{
public:
	dFace();
	virtual ~dFace();
	virtual bool IsFront();
	virtual void CalcNormal();

	virtual dVertex Normal();

	virtual void CalcZ();
	virtual float Z();
	
	virtual void Draw(dBitmap *bitmap);

private:
	float z;
	dVertex normal;
};

inline dVertex dFace::Normal()
{
	return normal;
}

#endif // _dFACE_