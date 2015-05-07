#include "PlayBoard.h"

PlayBoard::PlayBoard(HGE *hge, int skin, int level)
	:_hge(hge),
	_score(0),
	_state(P_START),
	_his_cnt(0),
	_time_left(ROUND_TIME),
	_skin(skin),
	_level(level){

	_play_music = hge->Effect_Load(PLAY_MUSIC);
	_play_music_channel = hge->Effect_PlayEx(_play_music);

	stringstream ss0;
	ss0 << "image/skin" << skin << ".jpg";
	skin_sprite = new hgeSprite(_hge->Texture_Load(ss0.str().c_str()), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  
	info_sprite = new hgeSprite(_hge->Texture_Load(PLAYINFO_IMAGE), 0, 0, 120, 300);

	memset(_his, 0, sizeof(_his));
	_jboard = new JewelBoard(_hge, _level);
	_timer = new hgeSprite(hge->Texture_Load(TIMER_IMAGE), 0, 0, TIMER_WIDTH, TIMER_HEIGHT);		
	_gui = new hgeGUI();
	
	_gui->AddCtrl(new MyButton(B_HINT, _hge, _hge->Texture_Load(HINT_IMAGE), 640, 50, 157, 73, _hge->Effect_Load(CLICK_MUSIC))); 
	_gui->AddCtrl(new MyButton(B_RESTART, _hge, _hge->Texture_Load(RESTART_IMAGE), 640, 150, 157, 66, _hge->Effect_Load(CLICK_MUSIC))); 
	_gui->AddCtrl(new MyButton(B_FRESH, _hge, _hge->Texture_Load(FRESH_IMAGE), 640, 250, 157, 73, _hge->Effect_Load(CLICK_MUSIC))); 
	_gui->AddCtrl(new MyButton(B_SUSPEND, _hge, _hge->Texture_Load(SUSPEND_IMAGE), 350, 570, 80, 80, _hge->Effect_Load(CLICK_MUSIC))); 
	_gui->AddCtrl(new MyButton(B_SAVE, _hge, _hge->Texture_Load(SAVE_IMAGE), 640, 350, 157, 73, _hge->Effect_Load(CLICK_MUSIC))); 
	_gui->AddCtrl(new MyButton(B_LOAD, _hge, _hge->Texture_Load(LOAD_IMAGE), 640, 450, 157, 73, _hge->Effect_Load(CLICK_MUSIC))); 
	_gui->AddCtrl(new MyButton(B_BACK, _hge, _hge->Texture_Load(BACK_IMAGE), 640, 550, 157, 73, _hge->Effect_Load(CLICK_MUSIC))); 
	
	_gui->AddCtrl(new MyButton(B_RESUME, _hge, _hge->Texture_Load(RESUME_IMAGE), 350, 570, 80, 80, _hge->Effect_Load(CLICK_MUSIC))); 
	_gui->EnableCtrl(B_RESUME, false);
	_gui->ShowCtrl(B_RESUME, false);
	
	hgeFont *fnt = new hgeFont("font/font1.fnt");//AdobeGothicStd-Bold.otf");
	fnt->SetScale(1);
	_level_text = new hgeGUIText(21, 30, 210, 100, 50, fnt);
	_level_text->SetMode(HGETEXT_CENTER);
	_level_text->SetColor(0xFF111111);
	_gui->AddCtrl(_level_text);

	_timer_text = new hgeGUIText(21, 30, 300, 100, 50, fnt);
	_timer_text->SetMode(HGETEXT_CENTER);
	_timer_text->SetColor(0xFF111111);//FFFFE060);//0xFFD4C25A);
	_gui->AddCtrl(_timer_text);

	_score_text = new hgeGUIText(20, 30, 350, 100, 100, fnt);
	_score_text->SetMode(HGETEXT_CENTER);
	_score_text->SetColor(0xFF111111);
	_gui->AddCtrl(_score_text);

	_gui->Enter();

}

void PlayBoard::render_func()
{
	skin_sprite->Render(0, 0);
	info_sprite->Render(10, 150);

	_jboard->render_func();
	_timer->RenderStretch(0, 540, (float)(TIMER_WIDTH)/ROUND_TIME*_time_left, 540+TIMER_HEIGHT);

	stringstream ss0;
	ss0 << _level;
	_level_text->SetText(ss0.str().c_str());	

	stringstream ss1;
	ss1 << _score;
	_score_text->SetText(ss1.str().c_str());

	stringstream ss2;
	ss2 << _time_left << "s";
	_timer_text->SetText(ss2.str().c_str());

	_gui->Render();
}

void PlayBoard::set_state(PLAYBOARD_STATE state)
{
	_state = state;
}

void PlayBoard::save()
{
	ofstream hisfile;
	hisfile.open("history.txt");
	hisfile << _level << " " << _time_left << " " << _score << endl;
	hisfile.close();
	_jboard->save();
}

void PlayBoard::load()
{
	ifstream load_file;
	load_file.open("history.txt");
	load_file >> _level >> _time_left >> _score;
	load_file.close();
	_jboard->load();
}

void PlayBoard::add_his()
{
	_his[_his_cnt] = new JewelBoard(*_jboard);
	_his_tleft[_his_cnt] = _time_left;
	_his_cnt++;
}

bool PlayBoard::frame_func()
{
	float dt = _hge->Timer_GetDelta();
	int id = _gui->Update(dt);
	if (_state == P_REPLAY_START)
		_time_left = ROUND_TIME;
	if (_time_left <= 0){
		add_his();
		set_state(P_END);
	}
	switch (id){
	case B_HINT:
		_jboard->find_hint();
		break;
	case B_RESTART:
		_score = 0;
		_time_left = ROUND_TIME;
		memset(_his, 0, sizeof(_his));
		_his_cnt = _his_cur = 0;
		_jboard = new JewelBoard(_hge, _level);
		break;
	case B_BACK:
		set_state(P_LEAVE);
		break;
	case B_FRESH:
		_jboard = new JewelBoard(_hge, _level);
		break;
	case B_RETRACT:
		break;

	case B_SUSPEND:
		_gui->ShowCtrl(B_SUSPEND, false);
		_gui->EnableCtrl(B_SUSPEND, false);
		_gui->ShowCtrl(B_RESUME, true);
		_gui->EnableCtrl(B_RESUME, true);
		_hge->Channel_Pause(_play_music_channel);
		set_state(P_SUSPEND);
		break;
	case B_RESUME:
		_gui->ShowCtrl(B_SUSPEND, true);
		_gui->EnableCtrl(B_SUSPEND, true);
		_gui->ShowCtrl(B_RESUME, false);
		_gui->EnableCtrl(B_RESUME, false);
		_hge->Channel_Resume(_play_music_channel);
		set_state(P_PLAY);
		break;
	}
	if (id == B_SAVE || _hge->Input_KeyUp(HGEK_F1))
		save();
	if (id == B_LOAD || _hge->Input_KeyDown(HGEK_F2))
		load();
	int tmp;

	switch (_state){
	case P_START:
		set_state(P_PLAY);
		return false;
		break;
	case P_PLAY:{
		_time_left -= dt;
		tmp = _jboard->frame_func();
		if (tmp > 1){
			add_his();	//record
			if (tmp == 2)//explode
				_score += _jboard->new_score();
		}
		return false;
		break;
				}
	case P_REPLAY_START:{
		_play_music_channel = _hge->Effect_PlayEx(_play_music);
		add_his();
		_jboard = new JewelBoard(*_his[0]);
		_time_left = ROUND_TIME;
		_score = 0;
		_his_cur = 0;
		set_state(P_REPLAY);
		break;
						}
	case P_REPLAY:{
		_time_left -= dt;
		tmp = _jboard->frame_func();
		if (tmp == 2)//explode
			_score += _jboard->new_score();

		if (abs(_time_left - _his_tleft[_his_cur]) < 0.01){
			_jboard = new JewelBoard(*_his[_his_cur]);
			_his_cur++;
			if (_his_cur >= _his_cnt){
				set_state(P_END);
				return false;
				break;
			}
		}
		break;
				  }
	case P_LEAVE:
		_hge->Channel_Pause(_play_music_channel);
		return true;
	case P_END:
		_hge->Channel_Pause(_play_music_channel);
		return true;
	}
	return false;
}
