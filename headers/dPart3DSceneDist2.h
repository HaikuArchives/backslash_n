#ifndef _dPART_3DSCENE_DIST2_
#define _dPART_3DSCENE_DIST2_

#include "dPart.h"

class dPart3DSceneDist2 : public dPart
{
public:
	dPart3DSceneDist2(dObject *obj1,dObject *obj2,dBitmap *background,float dist);
	virtual ~dPart3DSceneDist2();
	
	virtual void Render(dBitmap *bitmap,float time);
	void SetBackground(dBitmap *bitmap);
	float amount;
	
private:
	dObject *object1;
	dObject *object2;
	
	dObject *morph1;
	dObject *morph2;
	
	dBitmap *bitmap;
	float distance;
};

inline void dPart3DSceneDist2::SetBackground(dBitmap *map)
{
	bitmap = map;
}

#endif
