#include "JewelBoard.h"

//constructor
JewelBoard::JewelBoard(HGE *hge, int level):
	_hge(hge),
	_pre_x(-1),
	_pre_y(-1),
	_x(-1),
	_y(-1),
	_new_score(0),
	_level(level){
		memset(_map, 0, sizeof(_map));
		_click_music = _hge->Effect_Load(CLICK_MUSIC);
		_explode_music = _hge->Effect_Load(EXPLODE_MUSIC);
		for (int i = 0; i < COL; i++)
			for (int j = 0; j < ROW; j++)
				//for the initial board, there shouldn't be any explosion
				do{
					_map[i][j] = new Jewel(hge, posx(i, j), posy(i, j), MOVE_SPEED, 0, _level);
				}while (eliminate(i, j, false));
}

//copy constructor
JewelBoard::JewelBoard(const JewelBoard& pre)
	:_pre_x(pre._pre_x),
	_pre_y(pre._pre_y),
	_x(pre._x),
	_y(pre._y),
	_hge(pre._hge),
	_new_score(pre._new_score),
	_level(pre._level){
	for (int i = 0; i < COL; i++)
		for (int j = 0; j < ROW; j++)
			_map[i][j] = new Jewel(*(pre._map[i][j]));
}

//UI part
void JewelBoard::render_func()
{
	for (int i = 0; i < COL; i++)
		for (int j = 0; j < ROW; j++)
			if (_map[i][j])
				_map[i][j]->render_func();
}

//logic part
int JewelBoard::frame_func()
{
	//call each jewels frame_func()
	int ret = 1;	//all static
	for (int i = 0; i < COL; i++)
		for (int j = 0; j < ROW; j++)
			if (_map[i][j])	{
				_map[i][j]->frame_func();
				if (_map[i][j]->state() != J_LEISURE || _map[i][j]->state() != J_CLICK)
					ret = 0;	//some jewels are moving ex
			}

	//handle explosion
	if (can_explode()){
		explode();
		fall();
		re_generate();
		ret = 2;
		return ret;
	}

	//handle exchange
	if (_hge->Input_KeyUp(HGEK_LBUTTON)){//(_hge->Input_GetKeyState(HGEK_LBUTTON)){
		float px, py;
		_hge->Input_GetMousePos(&px, &py);
		if (px < JBDX || py < JBDY || px >= JBDX+JBDW || py >= JBDY+JBDH)
			//out of jewel board range
			return 0;
		_hge->Effect_Play(_click_music);
		_x = int((px-JBDX) / JWIDTH);
		_y = int((py-JBDY) / JHEIGHT);
		if (_map[_x][_y]->state() == J_LEISURE){
			if (!(_pre_x == -1 && _pre_y == -1))
				_map[_pre_x][_pre_y]->set_state(J_LEISURE);
			if ((_pre_x == -1 && _pre_y == -1) || (!is_adjacent(_pre_x, _pre_y, _x, _y))){
				_map[_x][_y]->set_state(J_CLICK);
				//it moves to a new position
				_pre_x = _x;
				_pre_y = _y;
			}else{
				//pre-swap to check whether they can trigger explosion
				swap(_map[_pre_x][_pre_y], _map[_x][_y]);
				if (can_explode()){	//if can
					swap(_map[_pre_x][_pre_y], _map[_x][_y]);
					exchange(_pre_x, _pre_y, _x, _y);
					ret = 3;
				}else{	//if cannot, swap back
					swap(_map[_pre_x][_pre_y], _map[_x][_y]);
					exchange(_pre_x, _pre_y, _x, _y);
					exchange(_pre_x, _pre_y, _x, _y);
				}
				_pre_x = -1;	//reset the board
				_pre_y = -1;
			}
		}
	}
	return ret;
}

//whether _map[px][py] & _map[qx][qy] are adjacent
//true for is, false otherwise
bool JewelBoard::is_adjacent(int px, int py, int qx, int qy)
{
	if (px == qx && abs(py - qy) == 1)
		return true;
	if (py == qy && abs(px - qx) == 1)
		return true;
	return false;
}

//exchange Jewel _map[px][py] & _map[qx][qy]
void JewelBoard::exchange(int px, int py, int qx, int qy)
{
	if (px != -1 && py != -1 && _map[px][py] && _map[qx][qy]) {
		_map[px][py]->move_to(posx(qx, qy), posy(qx, qy));
		_map[qx][qy]->move_to(posx(px, py), posy(px, py));
		swap(_map[px][py], _map[qx][qy]);
	}
}

//tag == false ~ check whether _map[x][y] can be eliminated
//tag == true  ~ do some pre-operations for explode()
bool JewelBoard::eliminate(int x, int y, bool tag)
{
	//check whether it can be eliminated first
	if (!_map[x][y])
		return false;
	int cnt[5];		//for each direction, how many jewels can be eliminated
	bool mark[5];	//for each direction, can it be eliminated?
	memset(cnt, 0, sizeof(cnt));
	memset( mark, 0, sizeof(mark));
	int stdtype = _map[x][y]->type();
	for (int i = 1; i <= 4; i++){
		int xx = x, yy = y;
		while (xx >= 0 && yy >= 0 && xx < COL && yy < ROW && _map[xx][yy] && _map[xx][yy]->type() == stdtype && (_map[xx][yy]->state() == J_LEISURE || _map[xx][yy]->state() == J_EXPLODE)){
			cnt[i]++;
			xx = xx + DX[i];
			yy = yy + DY[i];
		}
		if (i % 2 == 0 && cnt[i-1]+cnt[i] > 3){
			mark[i-1] = true;
			mark[i] = true;
		}
	}
	if (tag)	//do some pre-operations for explode()
		for (int i = 1; i <= 4; i++)
			if (mark[i]){
				int xx = x, yy = y;
				while (xx >= 0 && yy >= 0 && xx < COL && yy < ROW && _map[xx][yy] && _map[xx][yy]->type() == stdtype && (_map[xx][yy]->state() == J_LEISURE || _map[xx][yy]->state() == J_EXPLODE)){
					_map[xx][yy]->set_state(J_EXPLODE);
					xx = xx + DX[i];
					yy = yy + DY[i];
				}
		}
	return mark[1] | mark[3];
}

//whether some explosion can be triggered in this board
bool JewelBoard::can_explode()
{
	for (int i = 0; i < COL; i++)
		for (int j = 0; j < ROW; j++)
			if (_map[i][j]->state() == J_LEISURE && eliminate(i, j, false))
				return true;
	return false;
}

//do explosion, eliminating some jewels
void JewelBoard::explode()
{
	_hge->Effect_Play(_explode_music);
	_new_score = 0;
	for (int i = 0; i < COL; i++)
		for (int j = 0; j < ROW; j++)
			if (_map[i][j]->state() == J_LEISURE || _map[i][j]->state() == J_EXPLODE)
				eliminate(i, j, true);
	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COL; j++){
			if (_map[i][j]->state() == J_EXPLODE){
				_map[i][j] = NULL;
				_new_score += 10;
			}
		}
}

//fall down to fill the blank positions
void JewelBoard::fall()
{
	for (int j = ROW - 1; j >= 0; j--)
		for (int i = COL - 1; i >= 0; i--)
			if (!_map[i][j]){
				int x = i, y = j;
				while (y >= 0 && !_map[x][y])
					y--;		//up, find the nearest one in y axis
				if (y < 0)
					continue;
				_map[i][j] = _map[x][y];
				_map[i][j]->move_to(posx(i, j), posy(i, j));
				_map[x][y] = NULL;
		}
}

//re_generate some jewels to fill the blank positions
void JewelBoard::re_generate()
{
	//map[i][j] -- col i, row j
	for (int i = 0; i < ROW; i++){
		int lowest = 0;
		while (!_map[i][lowest])
			lowest++;
		for (int j = 0; j < COL; j++)
			if (!_map[i][j]){
				_map[i][j] = new Jewel(_hge, posx(i, j), -(lowest - j) * JHEIGHT, MOVE_SPEED, 0, _level);
				_map[i][j]->move_to(posx(i, j), posy(i, j));
			}
	}
}

void JewelBoard::find_hint()
{
	for (int i = 0; i < COL; i++)
		for (int j = 0; j < ROW; j++)
			for (int dir = 1; dir <= 4; dir++){	//four directions
				int pi = i + DX[dir], pj = j + DY[dir];
				if (pi >= 0 && pj >= 0 && pi <= COL && pj <= ROW){
					swap(_map[i][j], _map[pi][pj]);
					bool flag = eliminate(i, j, false);
					swap(_map[i][j], _map[pi][pj]);
					if (flag){
						if (_pre_x != -1 || _pre_y != -1){
							_map[_pre_x][_pre_y]->set_state(J_LEISURE);
						}
						_map[i][j]->set_state(J_CLICK);
						_pre_x = i;
						_pre_y = j;
						return;
					}
				}
			}		
}


//save JewelBoard
void JewelBoard::save()
{
	ofstream hisfile;
	hisfile.open("history.txt", ios::app);
	for (int i = 0; i < COL; i++){
		for (int j = 0; j < ROW; j++){
			hisfile << _map[i][j]->type() << " "
				<< _map[i][j]->x() << " " << _map[i][j]->y() << endl;
		}
	}
}

//reload the saved JewelBoard
void JewelBoard::load()
{
	ifstream load_file;
	load_file.open("history.txt");
	double BLAH;
	load_file >> BLAH >> BLAH >> BLAH;
	for (int i = 0; i < COL; i++)
		for (int j = 0; j < ROW; j++){
			int tmp; 
			double x, y;
			load_file >> tmp >> x >> y;
			_map[i][j] = new Jewel(_hge, x, y, MOVE_SPEED, tmp, _level);
			_map[i][j]->move_to(posx(i, j), posy(i, j));
		}
	load_file.close();
}