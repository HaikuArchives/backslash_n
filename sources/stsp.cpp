#include <stdio.h>
#include <SoundPlayer.h>
#include "stsp.h"

void stsp::BufferProc(void *player, void *buffer, size_t size, const media_raw_audio_format &format)
{ 
// only work for mono floating-point audio 
	if (format.channel_count != 1 || 
		format.format != media_raw_audio_format::B_AUDIO_FLOAT) { 
		puts("sorry, I only do mono-float");
		return; 
	} 
	((Song*)player)->Play((float*)buffer,size >> 2);	
}

stsp::stsp(char *filename):Song(filename)
{

#ifdef __INTEL__
	// mono-float
	media_raw_audio_format af = {	44100,
									1,
									media_raw_audio_format::B_AUDIO_FLOAT,
									B_MEDIA_LITTLE_ENDIAN,
									1024 };
#else
	// mono-float
	media_raw_audio_format af = {	44100,
									1,
									media_raw_audio_format::B_AUDIO_FLOAT,
									B_MEDIA_BIG_ENDIAN,
									1024 };
#endif
	player = new BSoundPlayer(&af, "SawTeeth", BufferProc, NULL, this); 
}

stsp::~stsp()
{
	delete player;
}

void stsp::Start()
{
	player->Start(); 
	player->SetHasData(true); 
}

void stsp::Stop()
{
	player->Stop();
}
