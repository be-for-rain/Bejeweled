#ifndef GameBoard_H_
#define GameBoard_H_

#include "hge.h"
#include "constants.h"
#include "Jewel.h"
#include "JewelBoard.h"
#include "MyMenu.h"
#include "PlayBoard.h"

//the boss of the game
//it switches its status bewteen menu & PlayBoard
class GameBoard{
public:
	static GameBoard *get_instance();	//there's only one instance, get it
private:
	GameBoard();				//it shouldn't be called by others
	~GameBoard();
	
	static bool frame_func();	//Logic part, call it every frame
	static bool render_func();	//UI part, call it every frame

	MyMenu *menu;
	PlayBoard *pboard;

	HGE *hge;
	GBOARD_STATE state;

	hgeGUIText *text_score;	//in order to display score in menu board
	hgeGUIText *text_level;	//in order to diaplay present level in menu board
	
	int skin;		//skin
	int level;		//level of difficulty
	HEFFECT snd;	//sound
	hgeFont *fnt;	//font

	HEFFECT _bg_music;
	HCHANNEL _bg_music_channel;

};

#endif