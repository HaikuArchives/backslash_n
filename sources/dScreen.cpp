#include <Application.h>

#include "dScreen.h"

dScreen::dScreen(const char *title,status_t *error):
		BWindowScreen(	title,
						B_8_BIT_640x480,
						error)
{
	disconnected=create_sem(0,"drawing_sem");
	ready_to_quit=create_sem(0,"sync_sem");

	clear_color = dColor(0);
	quitting = false;
	screen = new BScreen(this);
};	

dScreen::~dScreen()
{
	ASSERT(this != 0);

	delete screen;
	delete_sem(disconnected);
}

void dScreen::Quit()
{
//	printf("Quit() quitting = true\n");
	quitting = true;
//	printf("trying to acquire ready_to_quit sem\n");
	acquire_sem(ready_to_quit);
//	printf("Quit() acquired ready_to_quit sem\n");
	snooze(100000);
	BWindowScreen::Quit();
}

bool dScreen::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void dScreen::ScreenConnected(bool connected)
{
	ASSERT(this != 0);

	if (connected)
	{
		screen_base = (dColor*)CardInfo()->frame_buffer;

		dColor *base = screen_base;
		dColor *stop = screen_base + 640 * 480;

		while (base < stop)
		{
			*base = clear_color;
			base++;
		}
		
		release_sem(disconnected);
	}
	else
	{
//		printf("disconnected\n");
		acquire_sem(disconnected);
	}
}

bool dScreen::Draw(dBitmap *bitmap)
{
//	printf("Draw()\n");
	ASSERT(this != 0);
	ASSERT(bitmap != 0);
	ASSERT(bitmap->Width() == 640);
	ASSERT(bitmap->Height() == 400);

//	printf("quitting = %s\n",quitting?"true":"false");
	if (quitting)
	{
//		printf("Draw() quitting, releasing ready_to_quit sem\n");
		release_sem(ready_to_quit);
		return false;
	}

//	printf("Draw() trying to acquireing sem disconnected\n");
	while( acquire_sem_etc(disconnected,1,B_RELATIVE_TIMEOUT,10000) != B_NO_ERROR)
	{
//		printf("checking if we should quit..\n");
		if (quitting)
		{
//			printf("Draw() quitting, releasing ready_to_quit sem\n");
			release_sem(ready_to_quit);
			return false;
		}
	}
//	printf("Draw() disconnected sem acquired\n");

	double *base = (double*)(screen_base + 640 * 40);
	double *stop = (double*)(screen_base + 640 * 440);
	
	double *buffer = (double*)bitmap->Bits();
	
	while (base < stop)
	{
		base[0] = buffer[0];
		base[1] = buffer[1];
		base[2] = buffer[2];
		base[3] = buffer[3];
		base[4] = buffer[4];
		base[5] = buffer[5];
		base[6] = buffer[6];
		base[7] = buffer[7];

		base+=8;
		buffer+=8;
	}

	release_sem(disconnected);
//	printf("Draw() releasing sem disconnected\n");
	return true;
}

