#ifndef JewelBoard_H_
#define JewelBoard_H_

#include "Jewel.h"
#include "constants.h"
#include <iostream>
#include <sstream>
#include <fstream>

//a board to control 8*8 jewels
class JewelBoard{
public:
	JewelBoard(const JewelBoard& pre);	//copy constructor
	JewelBoard(HGE *hge, int level);	//constructer
	~JewelBoard();
	int frame_func();		//logic
	void render_func();		//UI
	const int new_score() { return _new_score; }
	
	Jewel *_map[COL][ROW];	//a map to store jewels

	void find_hint();	//find hint
	void save();	//save the game
	void load();	//reload saved game

private:
	void exchange(int px, int py, int qx, int qy);//exchange _map[px][py] & _map[qx][qy]
	bool can_explode();		//whether some explosion can be triggered in this board
	void explode();			//do exploding operation
	void fall();			//fall down to fill the blank positions
	void re_generate();		//re_generate some jewels to fill the blank positions

	bool is_adjacent(int px, int py, int qx, int qy); //whether _map[px][py] & _map[qx][qy] are adjacent
	bool eliminate(int x, int y, bool flag);	//check whether Jewel map[x][y] can be eliminated 


	int _pre_x, _pre_y;	//indicating where mouse is previously
	int _x, _y;	//indicating where mouse is now
	HGE *_hge;
	int _new_score;	//new score got only in this round
	int _level;	//choosed level of difficulty

	HEFFECT _click_music;
	HEFFECT _explode_music;

};

#endif