#include "dPart3DScene.h"

#include <File.h>
#include <Directory.h>
#include "dBitmap.h"

dPart3DScene::dPart3DScene(dObject *obj,dBitmap *background,float dist):dPart()
{
	bitmap = background;
	object = obj;
	distance = dist;
}

void dPart3DScene::Render(dBitmap *display,float time)
{
	object->MoveTo(sin(time/2),sin(time/3),sin(time/2) + distance);
	object->RotateTo(time/4,time/3,time/2);
	object->Project();

	display->AddObject(object);
	display->DrawBitmap(0,0,bitmap);
	display->Render();
}
