#include "dPart3DScene2.h"

#include <File.h>
#include <Directory.h>
#include "dBitmap.h"

dPart3DScene2::dPart3DScene2(dObject *obj1,dObject *obj2,dBitmap *background,float dist):dPart()
{
	bitmap = background;
	object1 = obj1;
	object2 = obj2;
	distance = dist;
}

void dPart3DScene2::Render(dBitmap *display,float time)
{
	object1->MoveTo(sin(time/2)*.2,sin(time/3)*.1 + .1,sin(time/2)*.2 + distance);
	object1->RotateTo(time/3 + 3.14,time/2 + 3.14,time/4);
	object1->Project();

	object2->MoveTo(sin(time/2)*.2,sin(time/3)*.1 + .1,sin(time/2)*.2 + distance);
	object2->RotateTo(time/3 + 3.14,time/2 + 3.14,time/4);
	object2->Project();

	display->AddObject(object1);
	display->AddObject(object2);
	display->DrawBitmap(0,0,bitmap);
	display->Render();
}
