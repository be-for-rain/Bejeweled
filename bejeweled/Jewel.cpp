#include "Jewel.h"
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>

//constructer
//x, y--its position, speed--its moving speed
Jewel::Jewel(HGE *hge, double x, double y, double speed, int type, int level)
	:_stage(0),
	_request(0),
	_speed(speed),
	_state(J_LEISURE),
	hgeSprite(0, 0, 0, JWIDTH, JHEIGHT){
		memset(_x, 0, sizeof(_x));
		memset(_y, 0, sizeof(_y));
		memset(_dir_move, 0, sizeof(_dir_move));

		_x[0] = x;
		_y[0] = y;
		_type = (type == 0) ? ((rand() % (5 + level))+1) : type; 
		//if type == 0, choose a type randomly
		//find the file name for this type of jewel
		stringstream ss;
		ss << "image/Jewel" << _type << ".png";
		HTEXTURE tex = hge->Texture_Load(ss.str().c_str());
		SetTexture(tex);	//rechange its texture through its type
}

Jewel::Jewel(const Jewel& dup)
	:hgeSprite(dup)
{
	_type = dup._type;
	stringstream ss;
	ss << "image/Jewel" << _type << ".png";
	HTEXTURE tex = hge->Texture_Load(ss.str().c_str());
	if(!tex){
		exit(0);
	}
	SetTexture(tex);	//rechange its texture through its type

	_stage = dup._stage;
	_request = dup._request;
	_speed = dup._speed;
	_state = dup._state;
	
	memcpy(_x, dup._x, sizeof(_x));
	memcpy(_y, dup._y, sizeof(_y));
	memcpy(_dir_move, dup._dir_move, sizeof(_dir_move));
}

Jewel::~Jewel()
{
}

//move by (delta_x, delta_y)
void Jewel::__move(double delta_x, double delta_y)
{
	_x[_stage] += delta_x;
	_y[_stage] += delta_y;
}

//move to (x, y)
void Jewel::move_to(double x, double y)
{
	_state = J_MOVE;
	_request++;
	_x[_request] = x;
	_y[_request] = y;
	_dir_move[_request-1] = get_direction(_x[_request], _y[_request], _x[_request-1], _y[_request-1]);
}

//change its state
void Jewel::set_state(JEWEL_STATE state)
{
	_state = state;
}

//UI part
void Jewel::render_func()
{
	//display a frame when clicked
	if (_state == J_CLICK){
		hgeSprite *tmp = new hgeSprite(hge->Texture_Load(CLICK_IMAGE), 0, 0, 60, 60);
		tmp->Render(_x[_stage], _y[_stage]);
	}
	Render(_x[_stage], _y[_stage]);	
}

//logic part
void Jewel::frame_func()
{
	switch (_state){
	case J_LEISURE:
		break;
	case J_CLICK:
		break;
	case J_EXPLODE:	//MARK: some animation can be added to it later
		break;
	case J_MOVE:
		__move(_speed * DX[_dir_move[_stage]], _speed * DY[_dir_move[_stage]]);
		if (same(_x[_stage], _x[_stage+1]) && same(_y[_stage], _y[_stage+1])){	//if it has reached its destination
			_stage++;
			if (_stage == _request){
				_state = J_LEISURE;
				_x[0] = _x[_stage];
				_y[0] = _y[_stage];
				_stage = _request = 0;
			}
		}
		break;
	}
}


/* ----------------helper function ----------------------------*/
//direction of moving (px, py) to (qx, qy)
//  ---->x
// |
// |
// \/
// y
DIRECTION get_direction(double px, double py, double qx, double qy)
{
	if (same(px, qx))	{ //px == qx aproximately
		if (same(py, qy))
			return NONE;	//it is the same point
		if (py < qy)
			return UP;
		return DOWN;
	}
	if (same(py, qy)){
		if (px < qx)
			return LEFT;
		return RIGHT;
	}
}
