#include "dPart3DSceneSpin2.h"

#include <File.h>
#include <Directory.h>
#include "dBitmap.h"

dPart3DSceneSpin2::dPart3DSceneSpin2(dObject *obj1,dObject *obj2,dBitmap *background,float dist):dPart()
{
	bitmap = background;
	object1 = obj1;
	object2 = obj2;
	distance = dist;

	object1->MoveTo(0.0,0.0,distance);
	object2->MoveTo(0.0,0.0,distance);
	
	object_spin1 = object1->Clone();
	object_spin2 = object2->Clone();

}

dPart3DSceneSpin2::~dPart3DSceneSpin2()
{
	delete object_spin1;
	delete object_spin2;
}

void dPart3DSceneSpin2::Render(dBitmap *display,float time)
{
	object1->MorphSpin(sin(time/2)*.3,object_spin1);
	object2->MorphSpin(sin(time/3)*.3,object_spin2);

	object_spin1->RotateTo(-time/2 - 1.0 + 3.1415,3.141,time/10);
	object_spin1->Project();

	object_spin2->RotateTo(time/2 - 1.0 + 3.1415,3.141,-time/10);
	object_spin2->Project();

	display->AddObject(object_spin1);
	display->AddObject(object_spin2);
	display->DrawBitmap(0,0,bitmap);
	display->Render();
}
