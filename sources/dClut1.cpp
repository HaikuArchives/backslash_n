#include "dClut1.h"

#include <File.h>
#include <File.h>

dClut1::dClut1()
{
	clut = new dColor[256*256];
}

dClut1::dClut1(BDirectory *dir,const char *filename)
{
	clut = new dColor[256*256];
	BFile file(dir,filename,B_READ_ONLY);
	file.Read(clut,256);
}

dClut1::~dClut1()
{
	delete []clut;
}