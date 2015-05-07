#include <ctime>
#include <iostream>
#include "hge.h"
#include "hgesprite.h"
#include "Jewel.h"
#include "JewelBoard.h"
#include "GameBoard.h"

int main() 
{
	srand(unsigned(time(NULL)));	//initialize it for rand()
	GameBoard *play = GameBoard::get_instance();	//get its only instance
	return 0;
}



