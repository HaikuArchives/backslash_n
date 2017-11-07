#ifndef _dCLUT1_
#define _dCLUT1_

#include <SupportDefs.h>
#include <Directory.h>
#include <File.h>

class dColor;

class dClut1
{
public:
	dClut1();
	dClut1(BDirectory *dir,const char *filename);
	virtual ~dClut1();
	
	void Set(uint8 index, dColor color);
	dColor Get(uint8 index);

private:
	dColor *clut;
};

#include "dColor.h"

inline void dClut1::Set(uint8 index, dColor color)
{
	clut[index] = color;
};

inline dColor dClut1::Get(uint8 index)
{
	return clut[index];
};

#endif
