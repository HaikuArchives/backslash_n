#ifndef _dPALETTE_
#define _dPALETTE_

#include <SupportDefs.h>
#include <GraphicsDefs.h>

#include <Directory.h>

class dPalette
{
public:
	dPalette();
	dPalette(BDirectory *dir,const char *filename);

	void Set(uint8 index, rgb_color color);
	rgb_color *Bits();

	dPalette operator*(float val);
	dPalette operator=(dPalette pal);

	dPalette Mix(rgb_color color,float weight);
	dPalette Mix(dPalette *pal1,float t);

//---------------------------------------------
	void Add(uint8 r, uint8 g, uint8 b);
	void Sub(uint8 r, uint8 g, uint8 b);

	rgb_color palette[256];

private:
};

#include "dColor.h"

inline dPalette dPalette::Mix(rgb_color color,float t)
{
	dPalette pal;
	for (int32 i=0;i<256;i++)
	{
		pal.palette[i].red = palette[i].red * (1.0 - t) + color.red * t;
		pal.palette[i].green = palette[i].green * (1.0 - t) + color.green * t;
		pal.palette[i].blue = palette[i].blue * (1.0 - t) + color.blue * t;
	}
	return pal;
}

inline dPalette dPalette::Mix(dPalette *pal1,float t)
{
	dPalette pal2;
	for (int32 i=0;i<256;i++)
	{
		pal2.palette[i].red = palette[i].red * (1.0 - t) + pal1->palette[i].red * t;
		pal2.palette[i].green = palette[i].green * (1.0 - t) + pal1->palette[i].green * t;
		pal2.palette[i].blue = palette[i].blue * (1.0 - t) + pal1->palette[i].blue * t;
	}
	return pal2;
}

inline dPalette dPalette::operator=(dPalette pal)
{
	for (int32 i=0;i<256;i++) palette[i] = pal.palette[i];
	return *this;
}

inline dPalette dPalette::operator*(float val)
{
	dPalette pal;
	for (int32 i=0;i<256;i++)
	{
		pal.palette[i].red = palette[i].red * val;
		pal.palette[i].green = palette[i].green * val;
		pal.palette[i].blue = palette[i].blue * val;
	}
	return pal;
}

inline void dPalette::Set(uint8 index, rgb_color color)
{
	ASSERT(this);
	ASSERT(index < 256);
	palette[index] = color;
}

inline rgb_color *dPalette::Bits()
{
	ASSERT(this);
	return palette;
}

inline void dPalette::Add(uint8 r, uint8 g, uint8 b)
{
	ASSERT(this);
	int tmp;
	for (int c = 0 ; c < 256 ; c++){
		tmp = palette[c].red + r;
		if (tmp > 255) tmp = 255;
		palette[c].red = tmp;

		tmp = palette[c].green + g;
		if (tmp > 255) tmp = 255;
		palette[c].green = tmp;

		tmp = palette[c].blue + b;
		if (tmp > 255) tmp = 255;
		palette[c++].blue = tmp;
	}
}

inline void dPalette::Sub(uint8 r, uint8 g, uint8 b)
{
	ASSERT(this);
	int tmp;
	for (int c = 0 ; c < 256 ; c++){
		tmp = palette[c].red - r;
		if (tmp < 0) tmp = 0;
		palette[c].red = tmp;

		tmp = palette[c].green - g;
		if (tmp < 0) tmp = 0;
		palette[c].green = tmp;

		tmp = palette[c].blue - b;
		if (tmp < 0) tmp = 0;
		palette[c].blue = tmp;
	}
}

#endif
