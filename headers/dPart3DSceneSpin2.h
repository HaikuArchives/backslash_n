#ifndef _dPART_3DSCENE_SPIN2_
#define _dPART_3DSCENE_SPIN2_

#include "dPart.h"

class dPart3DSceneSpin2 : public dPart
{
public:
	dPart3DSceneSpin2(dObject *obj1,dObject *obj2,dBitmap *background,float dist);
	virtual ~dPart3DSceneSpin2();
	virtual void Render(dBitmap *bitmap,float time);
private:
	dObject *object1;
	dObject *object2;
	
	dObject *object_spin1;
	dObject *object_spin2;
	
	dBitmap *bitmap;
	float distance;
};

#endif
