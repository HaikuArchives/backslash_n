#ifndef _dSCREEN_
#define _dSCREEN_

#include <WindowScreen.h>
#include <OS.h>
#include <Screen.h>
#include "dColor.h"
#include "dBitmap.h"
#include "dPalette.h"

class dScreen: public BWindowScreen
{
public:
	dScreen(const char *title,status_t *error);
	virtual ~dScreen();
	bool Draw(dBitmap *buffer);
	void SetPalette(dPalette *p);

	void WaitVbl();

	virtual void Quit();
	virtual bool QuitRequested();
private:
	BScreen *screen; // the BScreen object, used for monitor beam sync

	dColor *screen_base;
	dColor clear_color;
	sem_id disconnected; // is locked when the screen is disconnected
	sem_id ready_to_quit; // is released when the screen can delete itself
	bool quitting; // is true when the screen wants to quit

	virtual void ScreenConnected(bool connected);
};

inline void dScreen::SetPalette(dPalette *p)
{
	if (!quitting) SetColorList(p->Bits(), 0, 255);
}

inline void dScreen::WaitVbl()
{
	screen->WaitForRetrace();
}

#endif // _dSCREEN_