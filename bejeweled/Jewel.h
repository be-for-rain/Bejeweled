#ifndef Jewel_H_
#define Jewel_H_

#include <iostream>
#include "hge.h"
#include "hgesprite.h"
#include "constants.h"

using namespace std;

//regard a jewel as a hgeSprite
class Jewel: public hgeSprite{	
public:
	Jewel(HGE *hge, double x, double y, double speed, int type, int level); //constructor
	Jewel(const Jewel& pre);

	~Jewel();

	//get functions
	const int type() { return _type; }	//get its type
	const double x() { return _x[_stage]; }	//get its present x position
	const double y() { return _y[_stage]; }	//get its present y position
	const JEWEL_STATE state() { return _state; } //get its state

	//set function
	void set_state(JEWEL_STATE state);
	void move_to(double x, double y);	//move the jewel to (x, y)
	
	void frame_func();		//logic
	void render_func();		//UI : display the jewel
private:
	int _type;				//its type, there are 7 types of jewels in total
	double _x[10], _y[10];	//where it should be for each stage
	int _stage;				//which stage it is	
	int _request;			//how many stages are requested
	double _speed;			//its moving speed
	DIRECTION _dir_move[10];//its moving direction
	JEWEL_STATE _state;		//its current state : leisure/explode/move

	void __move(double delta_x, double delta_y);	//move by (delta_x, delta_y)

};

//helpler function
//direction of moving (px, py) to (qx, qy)
DIRECTION get_direction(double px, double py, double qx, double qy);

#endif