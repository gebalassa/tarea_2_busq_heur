#pragma once
#include <bitset>
#include <string>
#include "Constants.h"

using namespace std;

class Car {
public:
	Car(char id, bitset<SIZE> pos, bool isHor) : id(id), position(pos), isHorizontal(isHor) {};
	bitset<SIZE> position;
	char id;
	bool isHorizontal;
	bool checkMove(string move, bitset<SIZE> board) {};
	bitset<36> makeMove(string move, bitset<SIZE> board) {};

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

};