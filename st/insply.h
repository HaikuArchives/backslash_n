#ifndef INSPLY_H
#define INSPLY_H

#include "types.h"
#include "wave.h"

class Song;
class Ins;
class InsStep;

class InsPly
{
public:
	InsPly(Song *s);
	void PAL();
	float Next();

	void TrigADSR(uint8 i);
	void SetAmp(float a);
	void SetFreq(float f);
	void SetCutOff(float co);
private:

	Wave *vib,*pwm;
	float vamp,pamp;
	
	Song *song;
	Wave *w;
	Ins *ins;
	InsStep *currstep;
	uint8 stepc;		//instrument step
	int32 nexts;	//next ins

	Ins *currins;
	InsStep *steps;

	// Amp ADSR 
	float curramp;
	float ampstep;
	int8 adsr;
	int32 nextadsr;	

	// Filter ADSR 
	float currf;
	float fstep;
	int8 fadsr;
	int32 nextfadsr;	

	float llast;
	float last;
	float curr;

	// from inspattern
	float insfreq;
	
	// from player
	float currpartfreq;
	float currpartamp;
	float currpartco;
	
	// curr
	float res;
	float amp;
	float cutoff;
};

#endif
