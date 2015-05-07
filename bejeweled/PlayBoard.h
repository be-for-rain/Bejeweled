#ifndef PlayBoard_H_
#define PlayBoard_H_

#include "hge.h"
#include "hgegui.h"
#include "hgeguictrls.h"
#include "JewelBoard.h"
#include "MyButton.h"
#include <iostream>
#include <sstream>

//the control of JewelBoard
//calculate the left time & score got
//several buttons to control JewelBoard
class PlayBoard{
public:
	PlayBoard(HGE *hge, int skin, int level);	//constructor
	const PLAYBOARD_STATE state() { return _state; }	//constructor

	const int score() { return _score; }	//return its score
	void set_state(PLAYBOARD_STATE state);	//change its status

	bool frame_func();	//Logic
	void render_func();	//UI

private:
	void add_his();	//add history, in order to replay
	void save();	//save game
	void load();	//reload saved game
	void replay();


	PLAYBOARD_STATE _state;
		
	JewelBoard *_jboard;	//to control the jewelboard

	int _score;
	int _skin;
	int _level;
	float _time_left;
	int _his_cnt, _his_cur;

	JewelBoard *_his[200];
	float _his_tleft[200];
	
	HGE *_hge;
	hgeGUI *_gui;
	HEFFECT _play_music;
	HCHANNEL _play_music_channel;
	
	// to display some text & image
	hgeSprite *_timer;
	hgeSprite *skin_sprite;
	hgeSprite *info_sprite;
	hgeGUIText *_score_text;
	hgeGUIText *_timer_text;
	hgeGUIText *_level_text;
	
};




#endif