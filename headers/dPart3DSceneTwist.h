#ifndef _dPART_3DSCENE_TWIST_
#define _dPART_3DSCENE_TWIST_

#include "dPart.h"

class dPart3DSceneTwist : public dPart
{
public:
	dPart3DSceneTwist(dObject *obj1,dObject *obj2,dBitmap *background,float dist);
	virtual ~dPart3DSceneTwist();

	virtual void Render(dBitmap *bitmap,float time);
private:
	dObject *object1;
	dObject *object2;
	dObject *object_twist1;
	dObject *object_twist2;
	
	dBitmap *bitmap;
	float distance;
};

#endif
