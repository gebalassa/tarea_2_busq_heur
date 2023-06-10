#pragma once
#include <stdexcept>
#include "Car.cpp"


class HorizontalCar : public Car {
public:
	bitset<SIZE> leftmost;
	bitset<SIZE> rightmost;
	HorizontalCar(char id, bitset<SIZE> pos) : Car(id, pos, true) {
		//TODO: Initialize leftmost and rightmost
	}

	bool checkMove(string move, bitset<SIZE> board) {
		if (move == "left") {
			//TODO
		}
		else if (move == "right") {
			//TODO
		}
		else {
			throw invalid_argument("HorizontalCar.checkMove: MOV INVALIDO");
		}
	}

	// Check for collision, either with another car or with wall
	bool checkCollision(){
		//TODO
	}

	// Do move, return new table, update myself (position, "----most" vars)
	bitset<SIZE> makeMove(string move, bitset<SIZE> board) {
		//TODO
	}



};