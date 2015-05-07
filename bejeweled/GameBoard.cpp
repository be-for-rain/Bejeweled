#include "GameBoard.h"
#include "hge.h"

//there's only one instance, get it
GameBoard *GameBoard::get_instance()
{
	static GameBoard *instance = NULL;
	if (instance == NULL)
		instance = new GameBoard();
	return instance;
}

//Logic part, call it every frame
//it calls all its members' render_func()
bool GameBoard::render_func()
{
	GameBoard *tmp = GameBoard::get_instance();
	tmp->hge->Gfx_BeginScene();
	tmp->hge->Gfx_Clear(0x00000000);//0x00000000 for black, 0xFFFFFFFF for white
	switch (tmp->state){
	case G_MENU:
		if (tmp->menu->get_state() == M_NEWROUND){
			tmp->menu->render_func();		
			stringstream ss;
			ss << "You scored " << tmp->pboard->score() << "~!";
			tmp->text_score->SetText(ss.str().c_str());
			tmp->text_score->Render();
			stringstream ss2;
			ss2 << "Level " << tmp->level;
			tmp->text_level->SetText(ss2.str().c_str());
			tmp->text_level->Render();
		}else{
			tmp->menu->render_func();		
		}
		break;
	case G_PLAY:
		tmp->pboard->render_func();
		break;
	}
	tmp->hge->Gfx_EndScene();
	return false;
}

//UI part, call it every frame
//it calls all its members' frame_func()
//if returns true, the program will continue running right after hge->System_Start(); i.e. a way to exit
bool GameBoard::frame_func()
{
	GameBoard *tmp = GameBoard::get_instance();
	if (tmp->hge->Input_GetKeyState(HGEK_ESCAPE))
		return true;	//press "Esc" to exit
	if (tmp->state == G_MENU){
		int ret = tmp->menu->frame_func();
		switch (ret){
		case I_PLAY: case I_NEWROUND_PLAY:
			tmp->state = G_PLAY;
			tmp->hge->Channel_Stop(tmp->_bg_music_channel);		
			tmp->pboard = new PlayBoard(tmp->hge, tmp->skin, tmp->level);
			tmp->pboard->set_state(P_START);
			break;
		case I_REPLAY:
			tmp->state = G_PLAY;
			tmp->hge->Channel_Stop(tmp->_bg_music_channel);		
			tmp->pboard->set_state(P_REPLAY_START);
			break;
		case I_EXIT:
			return true;
			break;
		case I_LEVEL_0: case I_LEVEL_1: case I_LEVEL_2:
			tmp->level = ret - I_LEVEL_0;
			break;
		case I_SKIN_0: case I_SKIN_1: case I_SKIN_2:
			tmp->skin = ret - I_SKIN_0;
			break;
		case I_ADD:
			if (tmp->level < 2)
				tmp->level += 1;
			break;
		case I_SUBSTRACT:
			if (tmp->level > 0)
				tmp->level -= 1;
			break;
		}
	}else{	//tmp->state == G_PLAY
		if (tmp->pboard->frame_func()){
			tmp->state = G_MENU;
			tmp->_bg_music_channel = tmp->hge->Effect_Play(tmp->_bg_music);	
			if (tmp->pboard->state() == P_END)
				tmp->menu->set_state(M_NEWROUND);
			else
				tmp->menu->set_state(M_WELCOME);
		}
	}
	return false;
}

//constructor
GameBoard::GameBoard()
	:state(G_MENU),
	skin(0),
	level(0){
	hge = hgeCreate(HGE_VERSION);
	hge->System_SetState(HGE_SCREENWIDTH,WINDOW_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT,WINDOW_HEIGHT);
	hge->System_SetState(HGE_SHOWSPLASH,false); //don't display HGE Logo in welcome board
	hge->System_SetState(HGE_FRAMEFUNC,frame_func);
	hge->System_SetState(HGE_RENDERFUNC, render_func);
	hge->System_SetState(HGE_TITLE,"bejeweled");
	hge->System_SetState(HGE_WINDOWED,true);
	hge->System_SetState(HGE_USESOUND,true);	//NOTE: temporarily without music

	fnt=new hgeFont("font/font1.fnt");
	fnt->SetScale(1.2);
	
	_bg_music = hge->Effect_Load(MENU_MUSIC);
	_bg_music_channel = hge->Effect_Play(_bg_music);;
	

	text_score = new hgeGUIText(T_SCORE, 350, 100, 100, 100, fnt);
	text_score->SetColor(0xFFFFE060);
	text_score->SetMode(HGETEXT_CENTER);

	text_level = new hgeGUIText(T_SCORE, 350, 200, 100, 100, fnt);
	text_level->SetColor(0xFFFFE060);
	text_level->SetMode(HGETEXT_CENTER);

	hge->System_SetState(HGE_HIDEMOUSE, false); //do not hide mouse!
	menu = new MyMenu(hge);

	if (hge->System_Initiate())
		hge->System_Start();	

}

//destructor
GameBoard::~GameBoard()
{
	hge->Effect_Free(snd);
	hge->System_Shutdown();
	hge->Release();
	delete hge;
}
