#ifndef STSP
#define STSP
//////////////////////////////
#include <SoundPlayer.h>
#include "song.h"

class stsp : public Song {
public:
	stsp(char * filename = NULL);	
	~stsp();
	
	void	Start();
	void	Stop();
private:
	static void BufferProc(void *, void *, size_t , const media_raw_audio_format &);
	BSoundPlayer *player;
};
//////////////////////////////
#endif