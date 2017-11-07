#ifndef _dPART_
#define _dPART_

#include <SupportDefs.h>
#include <Directory.h>

#include "dBitmap.h"

class dPart
{
public:
	dPart();
	virtual ~dPart();
	
	virtual void Render(dBitmap *bitmap,float time);
private:
};

#endif
