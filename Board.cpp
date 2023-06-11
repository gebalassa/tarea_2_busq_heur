#pragma once
#include <iostream>
#include <bitset>
#include <vector>
#include <queue>
#include <stdexcept>
#include "Constants.h"
#include "HorizontalCar.cpp"
#include "VerticalCar.cpp"
#include "Car.cpp"

using namespace std;

class Board {
public:
	bitset<SIZE> board;
	vector<shared_ptr<Car>> cars;
	// IDs
	char idList[15] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','O','P' };
	int idCounter = 0;
	// Search
	class Compare {
	public:
		bool operator() (Board a, Board b) {
			return (a.score < b.score);
		}
	};
	priority_queue<Board, vector<Board>, Compare> children;
	int score;

	Board() {
		// TODO
		board = 0b0;
		cars = {};
		// CREAR AUTOS Y AGREGAR AL TABLERO
		// ??
		//????
	}

	// Agrega auto a autos y al tablero (retorna si fue exitoso)
	void addCar(bitset<SIZE> origin, int length, bool isHorizontal) {
		// Si la colocacion es valida...
		if (checkValidCarPlacement(origin, length, isHorizontal)) {
			// Crear auto
			// Horizontal
			if (isHorizontal) {
				// Crear posición nuevo auto
				bitset<SIZE> newCarPos = origin;
				for (int i = 1; i < length; i++) {
					newCarPos |= (origin << i);
				}
				// Creacion auto horizontal y agregar a 'cars'
				HorizontalCar newCar = HorizontalCar(idList[idCounter], newCarPos); idCounter++;
				shared_ptr<HorizontalCar> newCarPtr = make_shared<HorizontalCar>(newCar);
				cars.push_back(newCarPtr);
				// Sumar posición al mapa actual
				board |= newCar.position;
				// Creación exitosa
			}
			// Vertical
			else {
				// Crear posición nuevo auto
				bitset<SIZE> newCarPos = origin;
				for (int i = 1; i < length; i++) {
					newCarPos |= (origin << i * ROWSIZE);
				}
				// Creación auto vertical y agregar a 'cars'
				VerticalCar newCar = VerticalCar(idList[idCounter], newCarPos); idCounter++;
				shared_ptr<VerticalCar> newCarPtr = make_shared<VerticalCar>(newCar);
				cars.push_back(newCarPtr);
				// Sumar posición al mapa actual
				board |= newCar.position;
			}
		}
		else {
			cout << "ERROR: Colocacion de auto invalida!" << endl;
		}
	}

	// Retorna si ubicación de auto es válida
	bool checkValidCarPlacement(bitset<SIZE>& origin, int length, bool isHorizontal) {

		// Chequea que origen NO ES CERO
		if (origin.none()) { throw invalid_argument("_checkValidCarPlacement: origin bitset is zero!"); }
		// Chequea si origen esta ocupado
		if ((origin & board).any()) {
			return false;
		}
		// Chequea si cabe dentro de bordes (no traslapa hacia otra fila o afuera)
		if (isHorizontal) {

			int originIndex = index_from_bitset(origin);
			int leftmostIndex = originIndex + (length - 1);
			if (originIndex / ROWSIZE != leftmostIndex / ROWSIZE) {
				return false;
			}
		}
		else {
			int originIndex = index_from_bitset(origin);
			int uppermostIndex = originIndex + (length - 1) * ROWSIZE;
			if (uppermostIndex > SIZE - 1) {
				return false;
			}
		}
		// Chequea si casillas necesarias estan ocupadas
		for (int i = 1; i < length; i++) {
			if (isHorizontal) {
				bitset<SIZE> tempBoard = origin << i;
				if ((board & tempBoard).any()) { return false; }
			}
			else {
				bitset<SIZE> tempBoard = origin << i * ROWSIZE;
				if ((board & tempBoard).any()) { return false; }
			}
		}
		return true;
	}

	// Generador de movimientos
	void generateMoves() {
		// TODO
	}

	// Índice de pieza desde bitset
	bitset<SIZE> bitset_from_index(int raw_index) {
		if (raw_index < 0 || raw_index > 35) {
			throw invalid_argument("Index between 0-35.");
		}
		int index = raw_index;
		bitset<SIZE> canvas = 0b0;
		canvas[index] = 1;
		return canvas;
	}

	// Bitset desde índice de pieza
	static int index_from_bitset(bitset<SIZE>& piece) {
		unsigned long long index;
		if (piece.none()) {
			throw invalid_argument("Empty bitset!");
		}
		else {
			_find_piece_index(index, piece.to_ullong());
		}
		return (int)index;
	}
	// Búsqueda de índice shifteando a la derecha.
	static void _find_piece_index(unsigned long long& index, unsigned long long piece) {
		int counter = -1;
		if (piece == 0) { throw invalid_argument("Empty bitset!"); }
		else if (piece != 1 && piece % 2 != 0) { throw invalid_argument("Invalid piece board! Only single on-bit needed."); }
		while (piece != 0) { piece = piece >> 1; counter++; }
		index = counter;
	}

	// Imprime tablero entregado
	void print_board_letters(bitset<SIZE> board) {
		// Generación tablero con LETRAS de cada auto
		char tempBoard[SIZE]; for (int i = 0; i < SIZE; i++) { tempBoard[i] = '-'; }
		for (int i = 0; i < (int)cars.size(); i++) {
			// Letra origen del auto
			tempBoard[cars[i]->getOriginIndex()] = cars[i]->id;
			// Resto de posiciones del auto
			if (cars[i]->isHorizontal) {
				for (int j = 1; j < cars[i]->getLength(); j++) {
					tempBoard[cars[i]->getOriginIndex() + j] = cars[i]->id;
				}
			}
			else {
				for (int j = 1; j < cars[i]->getLength(); j++) {
					tempBoard[cars[i]->getOriginIndex() + j * ROWSIZE] = cars[i]->id;
				}
			}
		}
		// Imprimir
		cout << "+ 0 1 2 3 4 5" << "\n";
		cout << "0 ";
		for (int i = SIZE-1; i >= 0; i--) {
			if (i % ROWSIZE == 0 && i != 0) {
				cout << tempBoard[i] << "\n" << (ROWSIZE - i / ROWSIZE) << " ";
			}
			else if (i == 0) { cout << tempBoard[i] << "\n"; }
			else { cout << tempBoard[i] << " "; }
		}
	}
	// Por defecto
	void print_board_letters() { print_board_letters(board); }

	// Imprime tablero entregado
	void print_board(bitset<36> board) {
		cout << "+ 0 1 2 3 4 5" << "\n";
		cout << "0 ";
		for (int i = (int)board.size() - 1; i >= 0; i--) {
			if (i % ROWSIZE == 0 && i != 0) {
				cout << board[i] << "\n" << (ROWSIZE - i / ROWSIZE) << " ";
			}
			else if (i == 0) { cout << board[i] << "\n"; }
			else { cout << board[i] << " "; }
		}
	}
	// Por defecto
	void print_board() { print_board(board); }
};