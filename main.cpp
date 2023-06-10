#pragma once
#include "RushHourGame.cpp"
#include "Board.cpp"
#include "Constants.h"


// Main
int main() {
	// Board: checkValidCarPlacement
	Board b = Board();
	b.addCar(b.bitset_from_index(12), 7, true);
	//b.print_board();
	b.print_board_letters();




}