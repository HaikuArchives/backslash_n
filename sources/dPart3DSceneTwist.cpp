#include "dPart3DSceneTwist.h"

#include <File.h>
#include <Directory.h>
#include "dBitmap.h"

dPart3DSceneTwist::dPart3DSceneTwist(dObject *obj1,dObject *obj2,dBitmap *background,float dist):dPart()
{
	bitmap = background;
	object1 = obj1;
	object2 = obj2;
	distance = dist;
	object_twist1 = object1->Clone();
	object_twist2 = object2->Clone();
}

dPart3DSceneTwist::~dPart3DSceneTwist()
{
	delete object_twist1;
	delete object_twist2;
}

void dPart3DSceneTwist::Render(dBitmap *display,float time)
{
	object1->MorphSpin2(time,object_twist1);
	object2->MorphSpin2(time,object_twist2);

	object_twist1->MoveTo(sin(time/2)*.3,sin(time/3)*.1 + .1,sin(time/2)*.2 + distance);
	object_twist1->RotateTo(time/4,time/3,time/2);
	object_twist1->Project();

	object_twist2->MoveTo(sin(time/2)*.3,sin(time/3)*.1 + .1,sin(time/2)*.2 + distance);
	object_twist2->RotateTo(time/4,time/3,time/2);
	object_twist2->Project();

	display->AddObject(object_twist1);
	display->AddObject(object_twist2);

	display->DrawBitmap(0,0,bitmap);
	display->Render();
}
