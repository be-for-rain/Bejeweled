#ifndef MyMenu_H_
#define MyMenu_H_

#include "hge.h"
#include "hgegui.h"
#include "hgeguictrls.h"
#include "menuitem.h"
#include "constants.h"
#include <cmath>

//a class for several menu boards
//state indicates which board it is now
class MyMenu{
public:
	MyMenu(HGE *hgee);
	const MENU_STATE get_state() { return state; }
	int frame_func();
	bool render_func();
	void set_state(MENU_STATE new_state);
private:
	HGE *hge;
	MENU_STATE state;
	// Some resource handles
	HTEXTURE			tex;
	HEFFECT				snd;
	hgeQuad				quad;

	// Pointers to the HGE objects we will use
	hgeGUI				*gui;
	hgeFont				*fnt;
	hgeSprite			*spr, *skin0, *skin1, *skin2, *help;
	hgeGUIText			*text_bejeweled, *text_help;
	int id;
};

#endif