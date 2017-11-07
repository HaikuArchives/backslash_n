#ifndef _dPART_3DSCENE_
#define _dPART_3DSCENE_

#include "dPart.h"

class dPart3DScene : public dPart
{
public:
	dPart3DScene(dObject *obj,dBitmap *background,float dist);
	
	virtual void Render(dBitmap *bitmap,float time);
private:
	dObject *object;
	dBitmap *bitmap;
	float distance;
};

#endif
