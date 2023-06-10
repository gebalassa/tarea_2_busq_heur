#pragma once
#include <stdexcept>
#include "Car.cpp"

using namespace std;

class VerticalCar : public Car {
public:
	bitset<SIZE> uppermost;
	bitset<SIZE> lowermost;
	VerticalCar(char id, bitset<SIZE> pos) : Car(id, pos, false) {
		//TODO: Initialize uppermost and lowermost
	}

	// Check if move is possible
	bool checkMove(string move, bitset<SIZE> board) {
		if (move == "up") {
			//TODO
		}
		else if (move == "down") {
			//TODO
		}
		else {
			throw invalid_argument("VerticalCar.checkMove: MOV INVALIDO");
		}
	}

	// Check for collision, either with another car or with wall
	bool checkCollision(string move, bitset<SIZE> board) {
		//TODO
	}

	// Do move, return new table, update myself (position, "----most" vars)
	bitset<SIZE> makeMove(string move, bitset<SIZE> board) {
		//TODO
	}
};