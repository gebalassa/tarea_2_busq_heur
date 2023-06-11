#pragma once
#include <stdexcept>
#include "Car.cpp"

using namespace std;

class VerticalCar : public Car {
public:
	int uppermost;
	int lowermost;
	VerticalCar(char id, bitset<SIZE> pos) : Car(id, pos, false) {
		uppermost = indexLast();
		lowermost = indexFirst();
	}

	// Check if move is possible
	bool checkMove(string move, bitset<SIZE>& board) {
		if (move == "up") {
			// Checking borders
			if (uppermost+ROWSIZE > SIZE-1) {
				return false;
			}
			// Check other cars
			else if (board[uppermost+ROWSIZE] == 1) {
				return false;
			}
			return true;
		}
		else if (move == "down") {
			// Checking borders
			if (lowermost-ROWSIZE < 0) {
				return false;
			}
			// Check other cars
			else if (board[lowermost-ROWSIZE] == 1) {
				return false;
			}
			return true;
		}
		else {
			throw invalid_argument("VerticalCar.checkMove: MOV INVALIDO");
		}
	}

	// Do move on table, update myself (position, "----most" vars), return if successful
	bool makeMove(string move, bitset<SIZE>& board) {
		if (checkMove(move, board)) {
			// Modifico tablero entregado y mi posicion
			bitset<SIZE> temp = board ^ position; // Borro pos original
			if (move == "up") {
				position = position << ROWSIZE; // Modifico pos
				uppermost = uppermost + ROWSIZE;
				lowermost = lowermost + ROWSIZE;
			}
			else if (move == "down") {
				position = position >> ROWSIZE; // Modifico pos
				uppermost = uppermost - ROWSIZE;
				lowermost = lowermost - ROWSIZE;
			}
			temp = temp | position; // Agrego nueva pos
			board = position; // Modifico tablero recibido
			return true;
		}
		return false;
	}
};