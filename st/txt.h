#ifndef TXT_H
#define TXT_H

#include <stdio.h>
#include "types.h"

#define BUFSIZE 100	

class txt
{
public:
	txt(char *filename,bool create = false);
	~txt();
	status_t InitCheck();
	
	// call this to create a binary file (zero-RLE)
	void IsBin(bool b = true);
	void IsRLE(bool r = true);
	
	// read
	int NextVal(int numbytes = 1);
	float NextFloat();
	char *NextString();	// NOT malloc:ed
	
	// write
	void Comment(char *txt);
	void Comment(int val);
	void Val(int val, int numbytes = 1);
	void String(char *txt);	// nullterminated

private:
	void AtomWrite(int val);	
	int  AtomRead();
		
	void jntok(char *b, char *m);
	FILE *f;
	bool open_for_reading;
	bool bin;
	bool rle;
	char buf[BUFSIZE];
	status_t error;

	bool	r_zero_mode;
	bool	w_zero_mode;
	int		r_zero_count;
	int		w_zero_count;
};

#endif