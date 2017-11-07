#ifndef _GAME_WINDOW_H
#define _GAME_WINDOW_H

#include <DirectWindow.h>
#include <OS.h>
#include <Accelerant.h>

#define MAX_CLIPPING_RECT_COUNT 64

class GameWindow : public BDirectWindow
{
public:
	GameWindow(BRect frame, const char *name,bool fullscreen=true);
	virtual ~GameWindow();

	// standard window member
	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage *message);
	virtual void Quit();

	// this is the hook controling direct screen connection
	void DirectConnected(direct_buffer_info *info);
	
	void SetFullScreen(bool fullscreen);

	bool IsVisible();

	virtual bool DrawBuffer(void *screen);
								// as soon as this method returns false the window reference
								// is invalid and should not be used anymore
								// by the time Draw() has returned false the window
								// object is being destructed

	uint16 FrameWidth();	// returns the width (in pixels) of the expected frame buffer
	uint16 FrameHeight();// returns the height (in pixels) of the expected frame buffer

protected:

	// this function is used to inforce a direct access context
	// modification.
	void SwitchContext(direct_buffer_info *info);

#pragma pack(1)
	struct bitfield_rgb16
	{
		unsigned b:5;
		unsigned g:6;
		unsigned r:5;
	};

	struct bitfield_rgb32
	{
		unsigned b:8;
		unsigned g:8;
		unsigned r:8;
		unsigned a:8;
	};

	struct bitfield_rgb32_big
	{
		unsigned a:8;
		unsigned r:8;
		unsigned g:8;
		unsigned b:8;
	};
#pragma pack()

	uint16 frame_width,
			 frame_height;
	uint32 screen_offset;
	
	// used to synchronise the drawing.
	sem_id drawing_lock;

	// used to synchronise the shutdown.
	sem_id ready_to_quit;
		
	// the depth and format of each pixel in the current screen
	int32 pixel_depth;
	color_space pixel_format;
		
	// base pointer of the screen, one per pixel_depth
	uint8 *draw_ptr8;
	uint16 *draw_ptr16;
	uint32 *draw_ptr32;

	// offset, in pixels, between two lines of the frame buffer.
	uint32 pixel_row;
	
	// clipping region, defined as a list of rectangle, including the
	// smaller possible bounding box. This application will draw only
	// in the 64 first rectangles of the region. Region more complex
	// than that won't be completly used.
	clipping_rect clipping_bound;
	clipping_rect clipping_list[MAX_CLIPPING_RECT_COUNT];
	uint32 clipping_list_count;

	BScreen *b_screen;
	display_mode desktop_mode;

	bool window_quit;
};

inline bool GameWindow::IsVisible()
{
	if (acquire_sem_etc(drawing_lock,1,B_RELATIVE_TIMEOUT,0) == B_WOULD_BLOCK) return false;
	release_sem(drawing_lock);
	return true;
}

#endif // _GAME_WINDOW_H
