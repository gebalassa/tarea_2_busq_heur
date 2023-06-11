#pragma once
#include <bitset>
#include <stdexcept>
#include <string>
#include "Constants.h"

using namespace std;

class Car {
public:
	Car(char id, bitset<SIZE> pos, bool isHor) : id(id), position(pos), isHorizontal(isHor) {};
	bitset<SIZE> position;
	char id;
	bool isHorizontal;
	virtual bool checkMove(string move, bitset<SIZE>& board) = 0;
	virtual bool makeMove(string move, bitset<SIZE>& board) = 0;

	// Obtener origen del vehiculo (ya sea el mas abajo o el mas a la derecha)
	int getOriginIndex() {
		int index = 0;
		bitset<SIZE> temp = position;
		if (position[0] == 1) { return 0; }
		else {
			while (true)
			{
				temp = temp >> 1; index++;
				if (temp[0] == 1) { break; }
			}
			return index;
		}
	}

	// Obtener largo del vehiculo
	int getLength() {
		int counter = 0;
		bitset<SIZE> temp = position;
		if (position[0] == 1) { counter++; }
		while (!temp.none()) {
			temp = temp >> 1;
			if (temp[0] == 1) { counter++; }
		}
		return counter;
	}

	// Index for last on-bit in bitset
	int indexLast(bitset<SIZE> b) {
		if (b.none()) { throw invalid_argument("Empty bitset!"); }
		int counter = -1;
		bitset<SIZE> temp = b;
		while (temp.any()) {
			counter++;
			temp = temp >> 1;
		}
		return counter;
	}
	int indexLast() { return indexLast(position); }

	// Index for first on-bit in bitset
	int indexFirst(bitset<SIZE> b) {
		if (b.none()) { throw invalid_argument("Empty bitset!"); }
		int counter = 0;
		bitset<SIZE> temp = b;
		if (temp[0] == 1) { return 0; }
		else {
			while (temp[0] != 1) {
				counter++;
				temp = temp >> 1;
			}
		}
		return counter;
	}
	int indexFirst() { return indexFirst(position); }

};