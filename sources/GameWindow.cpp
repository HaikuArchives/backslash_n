#include "GameWindow.h"

#include <string.h>
#include <stdlib.h>

#include <AppFileInfo.h>
#include <GraphicsDefs.h>
#include <FindDirectory.h>
#include <Alert.h>
#include <File.h>
#include <Path.h>
#include <Application.h>

#include <Debug.h>
#include <Accelerant.h>

GameWindow::GameWindow(BRect frame, const char *name,bool fullscreen)
: BDirectWindow(frame, name, B_TITLED_WINDOW, B_NOT_ZOOMABLE + B_NOT_RESIZABLE)
{
	b_screen = NULL;
	window_quit=false;

	frame_width = (uint16)frame.Width()+1;
	frame_height = (uint16)frame.Height()+1;

	// allocate the semaphore used to synchronise the star animation drawing access.
	drawing_lock = create_sem(0, "screen locker");
	ready_to_quit = create_sem(0, "window locker");

	// Add a shortcut to switch in and out of fullscreen mode.
	AddShortcut('f', B_COMMAND_KEY, new BMessage('full'));
	AddShortcut(B_RETURN, B_COMMAND_KEY, new BMessage('full'));

	// If the graphic card/graphic driver we use doesn't support directwindow
	// in window mode, then we need to switch to fullscreen immediatly
	if (fullscreen || !BDirectWindow::SupportsWindowMode()) SetFullScreen(true);
}

GameWindow::~GameWindow()
{
//	if (IsFullScreen())
	if (b_screen != NULL)
		b_screen->SetMode(&desktop_mode,true);
}

void GameWindow::Quit()
{
	window_quit=true;
	acquire_sem(ready_to_quit);
	BDirectWindow::Quit();
}

bool GameWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void GameWindow::SetFullScreen(bool fullscreen)
{
	if (fullscreen)
	{
		b_screen=new BScreen(this);
		if (!b_screen->IsValid())
		{
			if (!SupportsWindowMode()) be_app->PostMessage(B_QUIT_REQUESTED);
			return;
		}

		b_screen->GetMode(&desktop_mode);
		
//		display_mode mode;


//		#define MODE_FLAGS	(B_SCROLL | B_8_BIT_DAC | B_HARDWARE_CURSOR | B_PARALLEL_ACCESS)
//		display_mode low={{ 25175, 640, 656, 752, 800, 480, 490, 492, 525, 0}, B_RGB32, 640, 480, 0, 0, MODE_FLAGS}; /* Vesa_Monitor_@60Hz_(640X480X8.Z1) */
//		display_mode high={{ 25175, 640, 656, 752, 800, 480, 490, 492, 525, 0}, B_RGB32, 1600,1200, 0, 0, MODE_FLAGS}; /* Vesa_Monitor_@60Hz_(640X480X8.Z1) */

//		b_screen->ProposeDisplayMode(&mode,&low,&high);


//		b_screen->SetMode(&mode);

		set_screen_space(0,B_32_BIT_640x480,false);

		BDirectWindow::SetFullScreen(true);
	}
	else
	{
		if (!SupportsWindowMode()) return;

		b_screen->SetMode(&desktop_mode);
		delete b_screen;
		b_screen=NULL;
		BDirectWindow::SetFullScreen(false);
	}
}

void GameWindow::MessageReceived(BMessage *message)
{
	int8		key_code;

	switch(message->what)
	{
	// Switch between full-screen mode and windowed mode.
	case 'full' :
		if (IsFullScreen()) SetFullScreen(false);
		else SetFullScreen(true);
		break;
	case B_KEY_DOWN :
		if (!IsFullScreen()) break;
		if (message->FindInt8("byte", &key_code) != B_OK) break;
		if (key_code == B_ESCAPE) PostMessage(B_QUIT_REQUESTED);
		break;
	default :
		BDirectWindow::MessageReceived(message);
		break;
	}
}

void GameWindow::DirectConnected(direct_buffer_info *info)
{
	// you need to use that mask to read the buffer state.
	switch (info->buffer_state & B_DIRECT_MODE_MASK)
	{
	// start a direct screen connection.
	case B_DIRECT_START :
//	fprintf(stderr,"B_DIRECT_START\n");
		SwitchContext(info);	// update the direct screen infos.
//		fprintf(stderr,"%d\n",info->clip_list[0].right);
		if (BDirectWindow::IsFullScreen())
		{
			if (frame_width<321 && frame_height<241)
			{
				uint16 x = ((clipping_bound.right+1)>>1)-frame_width;
				uint16 y = ((clipping_bound.bottom+1)>>1)-frame_height;
				screen_offset = y * pixel_row + x;
			}
			else
			{
				uint16 x = ((clipping_bound.right+1)>>1)-(frame_width>>1);
				uint16 y = ((clipping_bound.bottom+1)>>1)-(frame_height>>1);
				screen_offset = y * pixel_row + x;
			}
		}

//	fprintf(stderr,"/B_DIRECT_START\n");
		release_sem(drawing_lock);	// unblock the animation thread.
		break;
	// stop a direct screen connection.
	case B_DIRECT_STOP :
//	fprintf(stderr,"B_DIRECT_STOP\n");
		acquire_sem(drawing_lock);	// block the drawing.
//	fprintf(stderr,"/B_DIRECT_STOP\n");
		break;
	// modify the state of a direct screen connection.
	case B_DIRECT_MODIFY :
//	fprintf(stderr,"B_DIRECT_MODIFY\n");
		acquire_sem(drawing_lock);	// block the animation thread.
		SwitchContext(info);	// update the direct screen infos.
//	fprintf(stderr,"/B_DIRECT_MODIFY\n");
		release_sem(drawing_lock);	// unblock the animation thread.
		break;
	default :
		break;
	}
}

// This function update the internal graphic context of the GameWindow
// object to reflect the infos send through the DirectConnected API.
// It also update the state of stars (and erase some of them) to
// insure a clean transition during resize. As this function is called
// in DirectConnected, it's a bad idea to do any heavy drawing (long)
// operation. But as we only update the stars (the background will be
// updated a little later by the view system), it's not a big deal.
void GameWindow::SwitchContext(direct_buffer_info *info)
{
	// update to the new clipping region
	clipping_bound.left = info->clip_bounds.left;
	clipping_bound.right = info->clip_bounds.right;
	clipping_bound.top = info->clip_bounds.top;
	clipping_bound.bottom = info->clip_bounds.bottom;
	// the clipping count is bounded (see comment in header file).
	clipping_list_count = info->clip_list_count;
	if (clipping_list_count > MAX_CLIPPING_RECT_COUNT)
		clipping_list_count = MAX_CLIPPING_RECT_COUNT;
	for (uint16 i=0; i<clipping_list_count; i++)
	{
		clipping_list[i].left = info->clip_list[i].left - info->window_bounds.left;
		clipping_list[i].right = info->clip_list[i].right - info->window_bounds.left;
		clipping_list[i].top = info->clip_list[i].top - info->window_bounds.top;
		clipping_list[i].bottom = info->clip_list[i].bottom - info->window_bounds.top;
		}

	// update the new pixel_row
	pixel_row = info->bytes_per_row/(info->bits_per_pixel/8);
	
	// update the screen bases (only one of the 3 will be used).
	draw_ptr8 = ((uint8*)info->bits) + pixel_row*info->window_bounds.top + info->window_bounds.left;
	draw_ptr16 = ((uint16*)info->bits) + pixel_row*info->window_bounds.top + info->window_bounds.left;
	draw_ptr32 = ((uint32*)info->bits) + pixel_row*info->window_bounds.top + info->window_bounds.left;

	if (info->buffer_state & B_BUFFER_RESET)
	{
	}

	pixel_format = info->pixel_format;
}

uint16 GameWindow::FrameWidth()
{
	return frame_width;
}

uint16 GameWindow::FrameHeight()
{
	return frame_height;
}

bool GameWindow::DrawBuffer(void *screen_buffer)
{
	// fprintf(stderr,"Draw() trying to acquire_se\n");
	if (window_quit)
	{
		release_sem(ready_to_quit);
		return false;
	}

//	if (acquire_sem_etc(drawing_lock,1,B_RELATIVE_TIMEOUT,0) == B_WOULD_BLOCK) return true;
	acquire_sem(drawing_lock);
	// fprintf(stderr,"Draw() sem acquired!\n");
	
	uint16 x,y;
	bitfield_rgb32 *buffer=(bitfield_rgb32 *)screen_buffer;

	if (IsFullScreen())
	{
//	fprintf(stderr,"FullScreenFrame\n");
		if (frame_width<321 && frame_height<241)
		{
			switch (pixel_format)
			{
/*
				case B_RGB32:
				{
					bitfield_rgb32 *buffer2=screen_buffer + frame_width;
					bitfield_rgb32 *screen=(bitfield_rgb32 *)draw_ptr32; // fills every second line
					bitfield_rgb32 *screen2=(bitfield_rgb32 *)draw_ptr32 + pixel_row; // fills the lines in between
					register bitfield_rgb32 pixel1;
					register bitfield_rgb32 pixel2;
					register bitfield_rgb32 pixel3;
					register bitfield_rgb32 pixel4;
					register uint16 temp;
					for (y=0;y<frame_height-1;y++)
					{
						for (x=0;x<frame_width-1;x++)
						{
							pixel1=*buffer;
							pixel2=*(buffer+1);
							pixel3=*buffer2;
							pixel4=*(buffer2+1);

							temp = pixel2.r + pixel1.r;
							pixel2.r = temp >> 1;
							temp = pixel2.g + pixel1.g;
							pixel2.g = temp >> 1;
							temp = pixel2.b + pixel1.b;
							pixel2.b = temp >> 1;

							temp = pixel3.r + pixel1.r;
							pixel3.r = temp >> 1;
							temp = pixel3.g + pixel1.g;
							pixel3.g = temp >> 1;
							temp = pixel3.b + pixel1.b;
							pixel3.b = temp >> 1;


							temp = pixel1.r + pixel2.r + pixel3.r + pixel4.r;
							pixel4.r = temp >> 2;
							temp = pixel1.g + pixel2.g + pixel3.g + pixel4.g;
							pixel4.g = temp >> 2;
							temp = pixel1.b + pixel2.b + pixel3.b + pixel4.b;
							pixel4.b = temp >> 2;

							*screen++=pixel1;
							*screen++=pixel2;
							*screen2++=pixel3;
							*screen2++=pixel4;
							buffer++;
							buffer2++;
						}
						buffer++;
						buffer2++;
						screen+=2;
						screen2+=2;
						screen+=(pixel_row-frame_width)<<1;
						screen2+=(pixel_row-frame_width)<<1;
					}
				} break;
*/

				case B_RGB32:
				{
					bitfield_rgb32 *screen=(bitfield_rgb32 *)draw_ptr32 + screen_offset; // fills every second line
					bitfield_rgb32 *screen2=(bitfield_rgb32 *)draw_ptr32 + screen_offset + pixel_row; // fills the lines in between
					register bitfield_rgb32 pixel;
					for (y=0;y<frame_height;y++)
					{
						for (x=0;x<frame_width>>2;x++)
						{
							pixel=buffer[0];
							screen[0]=pixel;
							screen[1]=pixel;
							screen2[0]=pixel;
							screen2[1]=pixel;

							pixel=buffer[1];
							screen[2]=pixel;
							screen[3]=pixel;
							screen2[2]=pixel;
							screen2[3]=pixel;

							pixel=buffer[2];
							screen[4]=pixel;
							screen[5]=pixel;
							screen2[4]=pixel;
							screen2[5]=pixel;

							pixel=buffer[3];
							screen[6]=pixel;
							screen[7]=pixel;
							screen2[6]=pixel;
							screen2[7]=pixel;

							screen+=8;
							screen2+=8;
							buffer+=4;
						}
						screen+=(pixel_row-frame_width)<<1;
						screen2+=(pixel_row-frame_width)<<1;
					}
				} break;

				case B_RGBA32_BIG :
				{
					register bitfield_rgb32 pixel;
					register bitfield_rgb32_big pixel2;
					bitfield_rgb32_big *screen=(bitfield_rgb32_big *)draw_ptr32 + screen_offset; // fills every second line
					bitfield_rgb32_big *screen2=(bitfield_rgb32_big *)draw_ptr32 + screen_offset + pixel_row; // fills the lines in between
					for (y=0;y<frame_height;y++)
					{
						for (x=0;x<frame_width>>2;x++)
						{
							pixel=buffer[0];
							pixel2.a=pixel.a;
							pixel2.r=pixel.r;
							pixel2.g=pixel.g;
							pixel2.b=pixel.b;

							screen[0]=pixel2;
							screen[1]=pixel2;
							screen2[0]=pixel2;
							screen2[1]=pixel2;

							pixel=buffer[1];
							pixel2.a=pixel.a;
							pixel2.r=pixel.r;
							pixel2.g=pixel.g;
							pixel2.b=pixel.b;
							screen[2]=pixel2;
							screen[3]=pixel2;
							screen2[2]=pixel2;
							screen2[3]=pixel2;

							pixel=buffer[2];
							pixel2.a=pixel.a;
							pixel2.r=pixel.r;
							pixel2.g=pixel.g;
							pixel2.b=pixel.b;
							screen[4]=pixel2;
							screen[5]=pixel2;
							screen2[4]=pixel2;
							screen2[5]=pixel2;

							pixel=buffer[3];
							pixel2.a=pixel.a;
							pixel2.r=pixel.r;
							pixel2.g=pixel.g;
							pixel2.b=pixel.b;
							screen[6]=pixel2;
							screen[7]=pixel2;
							screen2[6]=pixel2;
							screen2[7]=pixel2;

							screen+=8;
							screen2+=8;
							buffer+=4;
						}
						screen+=(pixel_row-frame_width)<<1;
						screen2+=(pixel_row-frame_width)<<1;
					}
				} break;
			}
		}
		else
		{
			switch (pixel_format)
			{
				case B_RGB32:
				{
					bitfield_rgb32 *screen=(bitfield_rgb32 *)draw_ptr32 + screen_offset;
					for (y=0;y<frame_height;y++)
					{
						for (x=0;x<frame_width>>3;x++)
						{
							screen[0]=buffer[0];
							screen[1]=buffer[1];
							screen[2]=buffer[2];
							screen[3]=buffer[3];
							screen[4]=buffer[4];
							screen[5]=buffer[5];
							screen[6]=buffer[6];
							screen[7]=buffer[7];
							screen+=8;
							buffer+=8;
						}
						screen+=pixel_row-frame_width;
					}
				} break;
				case B_RGBA32_BIG :
				{
					register bitfield_rgb32 pixel;
					register bitfield_rgb32_big pixel2;
					bitfield_rgb32_big *screen=(bitfield_rgb32_big *)draw_ptr32 + screen_offset;
					for (y=0;y<frame_height;y++)
					{
						for (x=0;x<frame_width>>2;x++)
						{
							pixel=buffer[0];
							pixel2.a=pixel.a;
							pixel2.r=pixel.r;
							pixel2.g=pixel.g;
							pixel2.b=pixel.b;

							screen[0]=pixel2;

							pixel=buffer[1];
							pixel2.a=pixel.a;
							pixel2.r=pixel.r;
							pixel2.g=pixel.g;
							pixel2.b=pixel.b;
							screen[1]=pixel2;

							pixel=buffer[2];
							pixel2.a=pixel.a;
							pixel2.r=pixel.r;
							pixel2.g=pixel.g;
							pixel2.b=pixel.b;
							screen[2]=pixel2;

							pixel=buffer[3];
							pixel2.a=pixel.a;
							pixel2.r=pixel.r;
							pixel2.g=pixel.g;
							pixel2.b=pixel.b;
							screen[3]=pixel2;

							screen+=4;
							buffer+=4;
						}
						screen+=(pixel_row-frame_width)<<1;
					}
				} break;
			}
		}
	}
	else
	{
//	fprintf(stderr,"WindowFrame\n");
		switch (pixel_format)
		{
		case B_RGB32:
		{
			for (uint16 i=0;i<clipping_list_count;i++)
			{
				uint16 width=clipping_list[i].right-clipping_list[i].left+1;
				uint16 height=clipping_list[i].bottom-clipping_list[i].top+1;
				//fprintf(stderr,"Drawing.. width: %d height: %d\n",width,height);

				bitfield_rgb32 *screen=(bitfield_rgb32 *)draw_ptr32+clipping_list[i].top*pixel_row+clipping_list[i].left;
				buffer=(bitfield_rgb32 *)screen_buffer + (clipping_list[i].top*frame_width + clipping_list[i].left);
				
				for (y=0;y<height;y++)
				{
					for (x=0;x<width;x++) *screen++=*buffer++;
					buffer+=frame_width-width;
					screen+=pixel_row-width;
				}
			}
		} break;
		case B_RGBA32_BIG :
		{
			register bitfield_rgb32 pixel;
			register bitfield_rgb32_big pixel2;
			for (uint16 i=0;i<clipping_list_count;i++)
			{
				uint16 width=clipping_list[i].right-clipping_list[i].left+1;
				uint16 height=clipping_list[i].bottom-clipping_list[i].top+1;

				bitfield_rgb32_big *screen=(bitfield_rgb32_big *)draw_ptr32+clipping_list[i].top*pixel_row+clipping_list[i].left;
				buffer=(bitfield_rgb32 *)screen_buffer + (clipping_list[i].top*frame_width + clipping_list[i].left);

				for (y=0;y<height;y++)
				{
					for (x=0;x<width;x++)
					{
						pixel=*buffer++;
						pixel2.a=pixel.a;
						pixel2.r=pixel.r;
						pixel2.g=pixel.g;
						pixel2.b=pixel.b;
						*screen++=pixel2;
					}
					buffer+=frame_width-width;
					screen+=pixel_row-width;
				}
			}
		} break;
/*
		case B_RGB32_BIG :
			((uint8*)&pixel32)[3] = 0x20; 
			((uint8*)&pixel32)[2] = 0xff; 
			((uint8*)&pixel32)[1] = 0x20; 
			((uint8*)&pixel32)[0] = 0xff;
			break;
*/
		case B_RGB16 :
		{
			register bitfield_rgb32 pixel;
			register bitfield_rgb16 pixel2;
			for (uint16 i=0;i<clipping_list_count;i++)
			{
				uint16 width=clipping_list[i].right-clipping_list[i].left+1;
				uint16 height=clipping_list[i].bottom-clipping_list[i].top+1;

				bitfield_rgb16 *screen=(bitfield_rgb16 *)draw_ptr16+clipping_list[i].top*pixel_row+clipping_list[i].left;
				buffer=(bitfield_rgb32 *)screen_buffer + (clipping_list[i].top*frame_width + clipping_list[i].left);
				for (y=0;y<height;y++)
				{
					for (x=0;x<width;x++)
					{
						pixel=*buffer++;
						pixel2.r=pixel.r>>3;
						pixel2.g=pixel.g>>2;
						pixel2.b=pixel.b>>3;
						*screen++=pixel2;
					}
					buffer+=frame_width-width;
					screen+=pixel_row-width;
				}
			}
		} break;
/*
		case B_RGB15 :
		case B_RGBA15 :
			pixel_depth = 16;
			((uint8*)&pixel16)[0] = 0xe0;
			((uint8*)&pixel16)[1] = 0x03;
			break;
*/
		case B_CMAP8 :
		{
			register bitfield_rgb32 pixel;
			for (uint16 i=0;i<clipping_list_count;i++)
			{
				uint16 width=clipping_list[i].right-clipping_list[i].left+1;
				uint16 height=clipping_list[i].bottom-clipping_list[i].top+1;
				
				uint8 *screen=draw_ptr8 + clipping_list[i].top*pixel_row+clipping_list[i].left;
				buffer=(bitfield_rgb32*)screen_buffer + (clipping_list[i].top*frame_width + clipping_list[i].left);
				for (y=0;y<height;y++)
				{
					for (x=0;x<width;x++)
					{
						pixel=*buffer++;
						*screen++=b_screen->IndexForColor(pixel.r,pixel.g,pixel.b);
					}
					buffer+=frame_width-width;
					screen+=pixel_row-width;
				}
			}
		} break;
/*
		case B_RGB16_BIG :
			pixel_depth = 16;
			((uint8*)&pixel16)[1] = 0xe0;
			((uint8*)&pixel16)[0] = 0x07;
			break;
		case B_RGB15_BIG :
		case B_RGBA15_BIG :
			pixel_depth = 16;
			((uint8*)&pixel16)[1] = 0xe0;
			((uint8*)&pixel16)[0] = 0x03;
			break;
		default:	// unsupported color space?
			f// fprintf(stderr,stderr, "ERROR - unsupported color space!\n");
			exit(1);
			break;
*/
		}
	}

	// release the direct screen access
	release_sem(drawing_lock);
	// fprintf(stderr,"Draw() sem released\n");
	if (window_quit)
	{
		release_sem(ready_to_quit);
		return false;
	}
	else return true;
}
