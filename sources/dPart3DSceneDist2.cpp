#include "dPart3DSceneDist2.h"

#include <File.h>
#include <Directory.h>
#include "dBitmap.h"

dPart3DSceneDist2::dPart3DSceneDist2(dObject *obj1,dObject *obj2,dBitmap *background,float dist):dPart()
{
	bitmap = background;
	object1 = obj1;
	object2 = obj2;
	distance = dist;
	
	morph1 = object1->Clone();
	morph2 = object2->Clone();
}

dPart3DSceneDist2::~dPart3DSceneDist2()
{
	delete morph1;
	delete morph2;
}

void dPart3DSceneDist2::Render(dBitmap *display,float time)
{
	object1->MorphDissolve(amount,morph1);
	object2->MorphDissolve(amount,morph2);

	morph1->MoveTo(sin(time/2),sin(time/3)*.3,sin(time/2) + distance);
	morph1->RotateTo(time/30 + 3.14,time/1 + 3.14,time/30);
	morph1->Project();

	morph2->MoveTo(sin(time/2),sin(time/3)*.3,sin(time/2) + distance);
	morph2->RotateTo(time/30 + 3.14,time/1 + 3.14,time/30);
	morph2->Project();

	display->AddObject(morph1);
	display->AddObject(morph2);
	if (bitmap) display->DrawBitmap(0,0,bitmap);
	display->Render();
}
