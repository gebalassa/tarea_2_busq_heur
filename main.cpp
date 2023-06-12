#pragma once
#include "RushHourGame.cpp"
#include "Board.cpp"
#include "Constants.h"


// Main
int main() {
	// Board: checkValidCarPlacement
	//Board b = Board();
	//b.addCar(b.bitset_from_index(33), 3, true);
	//b.addCar(b.bitset_from_index(15), 3, false);
	//b.addCar(b.bitset_from_index(6), 4, true);
	////b.print_board();
	//b.print_board_letters();

	// Car: makeMove()
	//Board b = Board();
	//b.addCar(b.bitset_from_index(21), 3, false);
	//b.addCar(b.bitset_from_index(8), 3, true);
	//b.print_board_letters();
	//// Vertical
	//shared_ptr<VerticalCar> auxVerticalCar = dynamic_pointer_cast<VerticalCar>(b.cars[0]);
	//cout << auxVerticalCar->uppermost << endl;
	//cout << auxVerticalCar->lowermost << endl;
	//auxVerticalCar->makeMove("down", b.board);
	//b.print_board_letters();
	//cout << auxVerticalCar->uppermost << endl;
	//cout << auxVerticalCar->lowermost << endl;
	//// Horizontal
	//auto auxHorizontalCar = dynamic_pointer_cast<HorizontalCar>(b.cars[1]);
	//cout << auxHorizontalCar->leftmost << endl;
	//cout << auxHorizontalCar->rightmost << endl;
	//auxHorizontalCar->makeMove("right", b.board);
	//b.print_board_letters();
	//cout << auxHorizontalCar->leftmost << endl;
	//cout << auxHorizontalCar->rightmost << endl;

	 //Board: Priority Queue
	//Board aux = Board();
	//Board b1 = Board();
	//Board b2 = Board();
	//Board b3 = Board();
	//b1.score = 100;
	//b2.score = 200;
	//b3.score = 150;
	//aux.children.push(b1);
	//aux.children.push(b2);
	//aux.children.push(b3);
	//while (!aux.children.empty()) {
	//	cout << aux.children.top().score << endl;
	//	aux.children.pop();
	//}

	// Board: GenerateMoves()
	Board b = Board();
	// 1 AUTO
	b.addCar(b.bitset_from_index(8), 3, true);
	b.addCar(b.bitset_from_index(14), 2, true);
	b.addCar(b.bitset_from_index(18), 3, false);
	b.addCar(b.bitset_from_index(0), 2, true);
	b.print_board_letters();
	b.generateMoves();
	int size = (int)b.children.size();
	cout << size << endl;
	for (int i = 0; i < size; i++) {
		Board aux = b.children.top();
		cout << endl;
		aux.print_board_letters();
		b.children.pop();
	}
}