#include "dClut2.h"

#include <File.h>
#include <Directory.h>
#include "dColor.h"

dClut2::dClut2()
{
	clut = new dColor[256*256];
}

dClut2::dClut2(BDirectory *dir,const char *filename)
{
	clut = new dColor[256*256];
	BFile file(dir,filename,B_READ_ONLY);
	file.Read(clut,256*256);
}

dClut2::~dClut2()
{
	delete []clut;
}
