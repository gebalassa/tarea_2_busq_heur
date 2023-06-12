#pragma once
#include "Car.cpp"


class HorizontalCar : public Car {
public:
	int leftmost;
	int rightmost;
	HorizontalCar(char id, bitset<SIZE> pos) : Car(id, pos, true) {
		leftmost = indexLast();
		rightmost = indexFirst();
	}

	// Do move on table, update myself (position, "----most" vars), return if successful
	bool makeMove(string move, bitset<SIZE>& board) {
		if (checkMove(move, board)) {
			// Modifico tablero entregado y mi posicion
			bitset<SIZE> temp = board ^ position; // Borro pos original
			if (move == "left") {
				position = position << 1; // Modifico pos
				leftmost = leftmost + 1;
				rightmost = rightmost + 1;
			}
			else if (move == "right") {
				position = position >> 1; // Modifico pos
				leftmost = leftmost - 1;
				rightmost = rightmost - 1;
			}
			temp = temp | position; // Agrego nueva pos
			board = temp; // Modifico tablero recibido
			return true;
		}
		return false;
	}

	bool checkMove(string move, bitset<SIZE>& board) {
		if (move == "left") {
			// Checking borders
			if ((leftmost + 1) / ROWSIZE != leftmost / ROWSIZE) {
				return false;
			}
			// Check other cars
			else if (board[leftmost+1] == 1) {
				return false;
			}
			return true;
		}
		else if (move == "right") {
			// Checking borders
			if (rightmost == 0 || (rightmost - 1) / ROWSIZE != rightmost / ROWSIZE) {
				return false;
			}
			// Check other cars
			else if (board[rightmost - 1] == 1) {
				return false;
			}
			return true;
		}
		else {
			throw invalid_argument("HorizontalCar.checkMove: MOV INVALIDO");
		}
	}



};