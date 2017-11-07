#ifndef _dLINE_
#define _dLINE_

#include "dVertex.h"
#include "dColor.h"
#include "dMaterial.h"
#include "dPolygon.h"

#include <OS.h>
#include <GraphicsDefs.h>

class dLine : public dPolygon
{
public:
	dLine();
	virtual ~dLine();
	virtual bool IsFront();

	virtual void CalcZ();
	virtual float Z();
	
	void SetVisible(bool is_visible);
	
	virtual void Draw(dBitmap *bitmap);

private:
	float z;
	bool visible;
};

// inline definitions

inline void dLine::SetVisible(bool is_visible)
{
	visible = is_visible;
}


#endif // _dLINE_