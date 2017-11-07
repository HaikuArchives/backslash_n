#ifndef _dMATERIAL_
#define _dMATERIAL_

#include "dColor.h"
#include "dClut2.h"

class dBitmap;

#include <OS.h>

enum material_flags
{
	MATERIAL_FLAT = 1,	  // --
	MATERIAL_TEXTURED = 2, // --
	MATERIAL_ENVMAPPED = 4, // --
	MATERIAL_CLUT = 8,
	
	MATERIAL_CLUT_FLAT				= MATERIAL_CLUT + MATERIAL_FLAT, // --
	MATERIAL_CLUT_TEXTURED			= MATERIAL_CLUT + MATERIAL_TEXTURED, //--
	MATERIAL_CLUT_ENVMAPPED			= MATERIAL_CLUT + MATERIAL_ENVMAPPED, //--
};

class dMaterial
{
public:
	dMaterial();

	material_flags type;

	dColor color;
	dBitmap *texture;
	dClut2 *clut;
};

#endif // _dMATERIAL_
