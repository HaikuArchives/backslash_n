#include "dPart3DSceneDots.h"

#include <File.h>
#include <Directory.h>
#include "dBitmap.h"

dPart3DSceneDots::dPart3DSceneDots(dObject *obj,dBitmap *background,dClut2 *clt,float dist):dPart()
{
	clut = clt;
	bitmap = background;
	object = obj;
	distance = dist;
	
	object_spin = object->Clone();
}

dPart3DSceneDots::~dPart3DSceneDots()
{
	delete object_spin;
}

void dPart3DSceneDots::Render(dBitmap *display,float time)
{
	object->MorphSpin2(time/20,object_spin);

	object_spin->MoveTo(sin(time/2),sin(time/3),sin(time/2) + distance);
	object_spin->RotateTo(time/4,time/3,time/2);
	object_spin->Project();

	display->DrawBitmap(0,0,bitmap);
	display->DrawSubPixelPoints(object_spin,clut);
}
