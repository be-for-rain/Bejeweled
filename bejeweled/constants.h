#ifndef constants_H_
#define constants_H_

#include <cmath>

#define JTYPECNT 6	//total types of jewels
#define ROW 8		//row of jewel board
#define COL 8		//col of jewel board
#define JWIDTH 60	//default width of a jewel
#define JHEIGHT 60	//default height of a jewel

#define WINDOW_WIDTH 800	//the width of window
#define WINDOW_HEIGHT 650	//the height of window

#define TIMER_WIDTH WINDOW_WIDTH - 200
#define TIMER_HEIGHT 30

#define JBDX 150	//JewelBoard's x coordinate
#define JBDY 50	//JewelBoard's y coordinate
#define JBDW COL * JWIDTH	//JewelBoard's width
#define JBDH ROW * JHEIGHT	//JewelBoard's height

#define ROUND_TIME 30	//time for a round

#define MOVE_SPEED 1	//default moving speed

#define posx(i, j) (JBDX + JWIDTH * (i))	//col i, row j
#define posy(i, j) (JBDY + JHEIGHT * (j))

//to avoid some errors due to floating number
#define EPS 0.1
#define same(x, y) (abs((x) - (y)) < EPS)

#define BG_IMAGE "image/bg_dup.png"//.jpg"
#define CLICK_IMAGE "image/click.png"//nwoso1_¸±±¾.jpg"//"clickframe.png"
#define BACK_IMAGE "image/back.png"
#define HINT_IMAGE "image/hint.png"
#define FRESH_IMAGE "image/fresh.png"
#define REPLAY_IMAGE "image/replay.png"
#define RETRACT_IMAGE "image/retract.png"
#define SUSPEND_IMAGE "image/suspend.png"
#define RESUME_IMAGE "image/resume.png"
#define SAVE_IMAGE "image/save.png"
#define LOAD_IMAGE "image/load.png"
#define RESTART_IMAGE "image/restart.png"
#define TIMER_IMAGE "image/timer.png"
#define PLAYINFO_IMAGE "image/playinfo.png"
#define HELP_IMAGE "image/help.png"

#define MENU_MUSIC "sound/SpringWater.mp3"
#define PLAY_MUSIC "sound/SummerWaltz.mp3"//Cyberworld.mp3"
#define CLICK_MUSIC "sound/click.wav"
#define EXPLODE_MUSIC "sound/explode.wav"

enum MENU_STATE { M_LEAVE, M_WELCOME, M_LEVEL, M_SKIN, M_HELP, M_NEWROUND};
enum JEWEL_STATE { J_LEISURE, J_CLICK,  J_EXPLODE, J_MOVE, J_MOUSEDOWN, J_MOUSEUP};
enum GBOARD_STATE {G_MENU, G_PLAY};//{ G_WELCOME, G_PLAY, G_OPTIONS, G_HELP, G_EXIT};
enum PLAYBOARD_STATE { P_ENTER, P_START, P_PLAY, P_REPLAY_START, P_REPLAY, P_SUSPEND, P_LEAVE, P_END};
enum DIRECTION {NONE, LEFT, RIGHT, UP, DOWN};
enum BUTTON_NAME { B_NONE, B_HINT, B_RESTART, B_FRESH, B_REPLAY, B_RETRACT, B_SUSPEND, B_RESUME, B_SAVE, B_LOAD, B_BACK};
enum ITEM { I_NONE, I_PLAY, I_LEVEL, I_SKIN, I_HELP, I_EXIT, I_LEVEL_0, I_LEVEL_1, I_LEVEL_2, I_LEVEL_BACK, I_SKIN_0, I_SKIN_1, I_SKIN_2, I_SKIN_BACK, I_HELP_BACK, I_ADD, I_SUBSTRACT, I_NEWROUND_PLAY, I_REPLAY, I_NEWROUND_BACK}; 
enum GUI_TEXT { T_WELCOME, T_HELP, T_SCORE, T_LEVEL};

//direction
const int DX[] = {0, -1, 1, 0, 0}; //left, right, up, down
const int DY[] = {0, 0, 0, -1, 1};


#endif