#ifndef _dCOLOR_
#define _dCOLOR_

#include <SupportDefs.h>
#include <Debug.h>

class dClut1;
class dClut2;

class dColor
{
public:
	dColor(uint8 color_index);
	dColor();

	dColor operator=(dColor color);
	void operator+=(dColor color);
	dColor operator+(dColor color);
	dColor Blend(dClut2 *clut,dColor color);
	dColor Op(dClut1 *clut);

private:
	uint8 index;
};

#include "dClut1.h"
#include "dClut2.h"

inline dColor::dColor(uint8 color)
{
	index = color;
}

inline dColor::dColor()
{
	index = 0;
}

inline dColor dColor::operator=(dColor color)
{
	return index = color.index;
}

inline void dColor::operator+=(dColor color)
{
/*
#ifdef __INTEL__
	asm("mov EAX,color.index");
	asm("mov EBX,index");
	asm("add AL,BL");
	asm("sbb BH,0");
	asm("or  AL,BH");
	asm("mov index,AL");
//parm [eax][ebx] value[al] modify[ebx];
#else
*/
	uint16 tmp = index + color.index;
	if (tmp>255) tmp = 255;
	index = tmp;
//#endif
}

inline dColor dColor::operator+(dColor color)
{
	uint32 clamp = index + color.index;
	return (clamp > 255) ? 255 : clamp;
}

inline dColor dColor::Blend(dClut2 *clut,dColor color)
{
	ASSERT(clut != 0);
	return clut->Get(index,color.index);
}

inline dColor dColor::Op(dClut1 *clut)
{
	ASSERT(clut != 0);
	return clut->Get(index);
}

#endif