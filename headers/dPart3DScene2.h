#ifndef _dPART_3DSCENE2_
#define _dPART_3DSCENE2_

#include "dPart.h"

class dPart3DScene2 : public dPart
{
public:
	dPart3DScene2(dObject *obj1,dObject *obj2,dBitmap *background,float dist);
	
	virtual void Render(dBitmap *bitmap,float time);
private:
	dObject *object1;
	dObject *object2;
	dBitmap *bitmap;
	float distance;
};

#endif
