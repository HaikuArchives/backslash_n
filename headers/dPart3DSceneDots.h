#ifndef _dPART_3DSCENE_DOTS_
#define _dPART_3DSCENE_DOTS_

#include "dPart.h"

class dPart3DSceneDots : public dPart
{
public:
	dPart3DSceneDots(dObject *obj,dBitmap *background,dClut2 *clt,float dist);
	virtual ~dPart3DSceneDots();

	virtual void Render(dBitmap *bitmap,float time);
private:
	dObject *object;
	dObject *object_spin;
	dBitmap *bitmap;
	float distance;
	dClut2 *clut;
};

#endif
