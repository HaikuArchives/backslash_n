#ifndef _dCLUT2_
#define _dCLUT2_

#include <SupportDefs.h>
#include <Directory.h>

class dColor;

class dClut2
{
public:
	dClut2();
	dClut2(BDirectory *dir,const char *filename);
	virtual ~dClut2();
	
	void Set(uint8 index1, uint8 index2, dColor color);
	dColor Get(uint8 index1, uint8 index2);

private:
	dColor *clut;
};

#include "dColor.h"

inline void dClut2::Set(uint8 index1, uint8 index2, dColor color)
{
	clut[(index1 << 8) | index2] = color;
}

inline dColor dClut2::Get(uint8 index1, uint8 index2)
{
	return clut[(index2 << 8) | index1];
}

#endif
