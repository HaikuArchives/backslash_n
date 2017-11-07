#ifndef _dVERTEX_
#define _dVERTEX_

#include <OS.h>
#include <math.h>

class dVertex
{
public:
	dVertex();
	dVertex(float x,float y,float z);

	void Project();
	void Rotate(float *t,dVertex *vec);	// only 4x4
	void RotateZ(float za,dVertex *vex);
	void RotateSimple(float *t,dVertex *vec);	// only 4x4, but uses only the rotation part (3x3)
	void Rotate3x3(float *t,dVertex *vec);
	void SetTo(float x,float y,float z);

	float Angle(dVertex vertex);

	float x,y,z;	// the coordinates

	static uint16 CalcZoom(uint8 fov,uint16 screen_x,uint16 screen_y,float *vert_angle,float *horiz_angle);

	dVertex operator=(dVertex vec);
	dVertex operator+=(dVertex vec);
	dVertex operator/=(int32 val);
	dVertex operator/(int32 val);
	dVertex operator*(int32 val);
	dVertex operator*(float val);
	dVertex operator+(dVertex vec);
	dVertex operator-(dVertex vec);
	bool operator==(dVertex vec);

	static uint16 scr_x,scr_y,zoom;
};

// inline definitions

inline dVertex::dVertex()
{
	x=0.0;
	y=0.0;
	z=0.0;
}

inline dVertex::dVertex(float x_,float y_,float z_)
{
	x=x_;
	y=y_;
	z=z_;
}

inline dVertex dVertex::operator=(dVertex vec)
{
	x=vec.x;
	y=vec.y;
	z=vec.z;
	return vec;
}

inline dVertex dVertex::operator+=(dVertex vec)
{
	x+=vec.x;
	y+=vec.y;
	z+=vec.z;
	return *this;
}

inline dVertex dVertex::operator+(dVertex vec)
{
	return dVertex(x+vec.x,y+vec.y,z+vec.z);
}

inline dVertex dVertex::operator-(dVertex vec)
{
	return dVertex(x-vec.x,y-vec.y,z-vec.z);
}

inline dVertex dVertex::operator*(int32 val)
{
	return dVertex(x*val,y*val,z*val);
}

inline dVertex dVertex::operator*(float val)
{
	return dVertex(x*val,y*val,z*val);
}

inline bool dVertex::operator==(dVertex vec)
{
	return x==vec.x && y==vec.y && z==vec.z;
}

inline dVertex dVertex::operator/=(int32 val)
{
	x/=val;
	y/=val;
	z/=val;
	return *this;
}

inline dVertex dVertex::operator/(int32 val)
{
	return dVertex(x/val,y/val,z/val);
}

inline float dVertex::Angle(dVertex vertex)
{
	return acos(x*vertex.x + y*vertex.y + z*vertex.z);
}

inline void dVertex::SetTo(float nx,float ny,float nz)
{
	x=nx;
	y=ny;
	z=nz;
}

inline void dVertex::RotateZ(float za,dVertex *vex)
{
	vex->x = cos(za) * x - sin(za) * y;
	vex->y = sin(za) * x + cos(za) * y;
	vex->z = z;
}

inline void dVertex::Project()
{
	x = 320.0 + (x * 300.0 / z);
	y = 160.0 + (y * 300.0 / z);
}

#endif // _dVERTEX_
