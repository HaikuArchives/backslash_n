#ifndef _dAPP_
#define _dAPP_

#include <Application.h>
#include "stsp.h"
#include "dPart3DSceneDist2.h"
#include "dPart3DScene2.h"
#include "dPart3DSceneTwist.h"
#include "dPart3DSceneSpin2.h"

class dApp: public BApplication, public stsp
{
public:

	struct parameter
	{
		dScreen *screen;
		dApp *app;
	};

	dApp();
	virtual ~dApp();
	int32 CurrentPart();
	
	virtual void Break(uint32 cmd);
	bool cycle_palette;
private:
	status_t	error;
	thread_id thread;
	
	int32 current_part;
	static int32 main_thread(void *parameter);
	dScreen *screen;

	parameter param;
// ––––––––––––––––––––––-

	dBitmap *frame1;
	dBitmap *frame2;

// palette
	dPalette *red_palette;
	dPalette *green_palette;
	dPalette *blue_palette;
	dPalette *yellow_palette;

// textures
	dBitmap *tex_generic;
	dBitmap *tex_greets;
	dBitmap *tex_credits;
	dBitmap *tex_linefeed;
	dBitmap *envmap;

// backgrounds
	dBitmap *flum;
	dBitmap *eyes;
	dBitmap *eyes2;
	dBitmap *logo;

// wipes
	dBitmap *wipe_hole;
	dBitmap *wipe_ripple;
	dBitmap *wipe_twirl;
	dBitmap *wipe_square;

// cluts
	dClut2 *clut_mix50;
	dClut2 *clut_mix70;
	dClut2 *clut_mix90;
	dClut2 *clut_screen60;
	dClut2 *clut_diff100;
	dClut2 *iclut_invmul;
	dClut2 *iclut_lighten;

// objects
	dObject *poles_big;
	dObject *cubes;
	dObject *ball_cred;
	dObject *ball_greet;
	dObject *linefeed_5;
	dObject *linefeed_5_w;
	dObject *cigarrs;
	dObject *cigarrs_c;
	dObject *poles;
	dObject *nut;
	dObject *nut_w;
	dContourObject *nut_c;
	dContourObject *poles_big_c;
	dContourObject *cubes_c;
	dContourObject *poles_c;

// scenes
	dPart3DScene2 *part1;
	dPart3DScene2 *part2;
	dPart3DScene2 *nut_scene;
	dPart3DSceneDist2 *cubes_scene;
	dPart3DSceneDist2 *poles_scene;
	dPart3DSceneDots *cut_scene;
	dPart3DSceneTwist *cigarr_scene;
	dPart3DSceneSpin2 *cred_scene;
	dPart3DSceneTwist *linefeed_scene;
	dPart3DSceneTwist *cubes_twist_scene;
};

// inline definitions

inline int32 dApp::CurrentPart()
{
	return current_part;
}

#endif // _dAPP_
