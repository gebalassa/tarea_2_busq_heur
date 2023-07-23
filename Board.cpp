#pragma once
#include <iostream>
#include <bitset>
#include <vector>
#include <queue>
#include <stdexcept>
#include "Constants.h"
#include "Statistics.h"
#include "HorizontalCar.cpp"
#include "VerticalCar.cpp"
#include "Car.cpp"


class Board {
public:
	bitset<SIZE> board;
	vector<shared_ptr<Car>> cars;
	// Victoria 
	static const int exitIndex = 18;
	// Jugador
	shared_ptr<HorizontalCar> playerCar;
	// IDs
	char idList[17] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q' };
	int idCounter = 0;
	// Search
	class Compare {
	public:
		bool operator() (const shared_ptr<Board> a, const shared_ptr<Board> b) {
			return (a->fscore < b->fscore);
		}
	};
	int hscore;
	int gscore;
	int fscore;
	priority_queue<shared_ptr<Board>, vector<shared_ptr<Board>>, Compare> children;
	shared_ptr<Board> parent;

	Board() {
		board = 0b0;
		cars = {};
		children = {};
		hscore = 0;
		gscore = 0;
		fscore = 0;
		parent = nullptr;
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
	// Alternativa
	void addCar(int originIndex, int length, bool isHorizontal) {
		if (originIndex > SIZE || originIndex < 0) { throw invalid_argument("addCar: Indice fuera de limite"); }
		bitset<SIZE> aux = bitset_from_index(originIndex);
		addCar(aux, length, isHorizontal);
	}


	// Agrega auto rojo del jugador
	void addPlayerCar(bitset<SIZE> origin, int length) {
		if (checkValidCarPlacement(origin, length, true)) {
			// Agrego a jugador
			addCar(origin, length, true);
			// Modifico variables necesarias
			playerCar = dynamic_pointer_cast<HorizontalCar>(cars[cars.size() - 1]);
			idCounter--; // Evito perder una letra de nombres
			playerCar->id = 'R'; // Agrego letra correcta ('R') a jugador

		}
	}
	// Alternativa
	void addPlayerCar(int originIndex, int length) {
		if (originIndex > SIZE || originIndex < 0) { throw invalid_argument("addPlayerCar: Indice fuera de limite"); }
		bitset<SIZE> aux = bitset_from_index(originIndex);
		addPlayerCar(aux, length);
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
				if ((board & tempBoard).any()) {
					return false;
				}
			}
			else {
				bitset<SIZE> tempBoard = origin << i * ROWSIZE;
				if ((board & tempBoard).any()) {
					return false;
				}
			}
		}
		return true;
	}

	// Generador de movimientos
	void generateMoves(shared_ptr<Board> parent) {
		// Por cada auto
		shared_ptr<Car> currCar;
		for (int i = 0; i < (int)cars.size(); i++) {
			currCar = cars[i];

			// Movimientos IZQ y DER
			if (currCar->isHorizontal) {
				shared_ptr<HorizontalCar> currHorizontalCar = dynamic_pointer_cast<HorizontalCar>(currCar);
				int movesToReverse = 0;

				// IZQUIERDA
				while (currHorizontalCar->makeMove("left", board)) {
					// Aumenta movimientos a revertir
					movesToReverse++;
					// Preparación nuevo hijo
					Board newBoard = Board();
					// Copiar autos al nuevo hijo
					for (int j = 0; j < (int)cars.size(); j++) {
						if (cars[j]->id == 'R') {
							newBoard.addPlayerCar(bitset_from_index(playerCar->getOriginIndex()), playerCar->getLength());
						}
						else {
							if (cars[j]->isHorizontal) {
								newBoard.addCar(bitset_from_index(cars[j]->getOriginIndex()), cars[j]->getLength(), true);
							}
							else {
								newBoard.addCar(bitset_from_index(cars[j]->getOriginIndex()), cars[j]->getLength(), false);
							}
						}
					}
					// Puntuación heurística
					newBoard.gscore = gscore - 1; // Puntuación disminuye a mayor distancia
					newBoard.hscore = newBoard.heuristicBlockingCars();
					newBoard.fscore = newBoard.gscore + newBoard.hscore;
					// Asignar padre al hijo
					newBoard.parent = parent;
					// Agregar nuevo hijo
					shared_ptr<Board> newBoardPtr = make_shared<Board>(newBoard);
					children.push(newBoardPtr);
				}
				// REINICIO A POSICION ORIGINAL usando moviento en dirección opuesta
				for (int j = 0; j < movesToReverse; j++) {
					currHorizontalCar->makeMove("right", board);
				}
				movesToReverse = 0;

				// DERECHA
				while (currHorizontalCar->makeMove("right", board)) {
					// Aumenta movimientos a revertir
					movesToReverse++;
					// Preparación nuevo hijo
					Board newBoard = Board();
					// Copiar autos al nuevo hijo
					for (int j = 0; j < (int)cars.size(); j++) {
						if (cars[j]->id == 'R') {
							newBoard.addPlayerCar(bitset_from_index(playerCar->getOriginIndex()), playerCar->getLength());
						}
						else {
							if (cars[j]->isHorizontal) {
								newBoard.addCar(bitset_from_index(cars[j]->getOriginIndex()), cars[j]->getLength(), true);
							}
							else {
								newBoard.addCar(bitset_from_index(cars[j]->getOriginIndex()), cars[j]->getLength(), false);
							}
						}
					}
					// Puntuación heurística
					newBoard.gscore = gscore - 1; // Puntuación disminuye a mayor distancia
					newBoard.hscore = newBoard.heuristicBlockingCars();
					newBoard.fscore = newBoard.gscore + newBoard.hscore;
					// Asignar padre al hijo
					newBoard.parent = parent;
					// Agregar nuevo hijo
					shared_ptr<Board> newBoardPtr = make_shared<Board>(newBoard);
					children.push(newBoardPtr);
				}
				// REINICIO A POSICION ORIGINAL usando moviento en dirección opuesta
				for (int j = 0; j < movesToReverse; j++) {
					currHorizontalCar->makeMove("left", board);
				}
				movesToReverse = 0;
			}
			// Movimientos ARRIBA y ABAJO
			//TODO
			else {
				shared_ptr<VerticalCar> currVerticalCar = dynamic_pointer_cast<VerticalCar>(currCar);
				int movesToReverse = 0;

				// ARRIBA
				while (currVerticalCar->makeMove("up", board)) {
					// Aumenta movimientos a revertir
					movesToReverse++;
					// Preparación nuevo hijo
					Board newBoard = Board();
					// Copiar autos al nuevo hijo
					for (int j = 0; j < (int)cars.size(); j++) {
						if (cars[j]->id == 'R') {
							newBoard.addPlayerCar(bitset_from_index(playerCar->getOriginIndex()), playerCar->getLength());
						}
						else {
							if (cars[j]->isHorizontal) {
								newBoard.addCar(bitset_from_index(cars[j]->getOriginIndex()), cars[j]->getLength(), true);
							}
							else {
								newBoard.addCar(bitset_from_index(cars[j]->getOriginIndex()), cars[j]->getLength(), false);
							}
						}
					}
					// Puntuación heurística
					newBoard.gscore = gscore - 1; // Puntuación disminuye a mayor distancia
					newBoard.hscore = newBoard.heuristicBlockingCars();
					newBoard.fscore = newBoard.gscore + newBoard.hscore;
					// Asignar padre al hijo
					newBoard.parent = parent;
					// Agregar nuevo hijo
					shared_ptr<Board> newBoardPtr = make_shared<Board>(newBoard);
					children.push(newBoardPtr);
				}
				// REINICIO A POSICION ORIGINAL usando moviento en dirección opuesta
				for (int j = 0; j < movesToReverse; j++) {
					currVerticalCar->makeMove("down", board);
				}
				movesToReverse = 0;

				//ABAJO
				while (currVerticalCar->makeMove("down", board)) {
					// Aumenta movimientos a revertir
					movesToReverse++;
					// Preparación nuevo hijo
					Board newBoard = Board();
					// Copiar autos al nuevo hijo
					for (int j = 0; j < (int)cars.size(); j++) {
						if (cars[j]->id == 'R') {
							newBoard.addPlayerCar(bitset_from_index(playerCar->getOriginIndex()), playerCar->getLength());
						}
						else {
							if (cars[j]->isHorizontal) {
								newBoard.addCar(bitset_from_index(cars[j]->getOriginIndex()), cars[j]->getLength(), true);
							}
							else {
								newBoard.addCar(bitset_from_index(cars[j]->getOriginIndex()), cars[j]->getLength(), false);
							}
						}
					}
					// Puntuación heurística
					newBoard.gscore = gscore - 1; // Puntuación disminuye a mayor distancia
					newBoard.hscore = newBoard.heuristicBlockingCars();
					newBoard.fscore = newBoard.gscore + newBoard.hscore;
					// Asignar padre al hijo
					newBoard.parent = parent;
					// Agregar nuevo hijo
					shared_ptr<Board> newBoardPtr = make_shared<Board>(newBoard);
					children.push(newBoardPtr);
				}
				// REINICIO A POSICION ORIGINAL usando moviento en dirección opuesta
				for (int j = 0; j < movesToReverse; j++) {
					currVerticalCar->makeMove("up", board);
				}
				movesToReverse = 0;
			}
		}
	}


	// A*
	vector<shared_ptr<Board>> aStar(Board& start) {
		// Estadisticas
		Statistics::instance->reset();
		//
		priority_queue<shared_ptr<Board>, vector<shared_ptr<Board>>, Compare> open = {};
		vector<shared_ptr<Board>> closed = {};
		start.fscore = 0;
		start.parent = nullptr;
		open.push(make_shared<Board>(start));

		// Loop
		while (!open.empty()) {
			shared_ptr<Board> currNode = open.top(); open.pop();
			// Generacion de hijos. Puntajes f,g,h y 'parent' se asignan aqui
			currNode->generateMoves(currNode);

			// DEBUG PRINT
			//for (int i = 0; i < 1; i++) {//currNode->children.size(); i++) {
			//	shared_ptr<Board> aux = currNode->children.top(); //children.pop();
			//	aux->print_board_letters();
			//}
			//cout << "\n=========================\n";
			// FIN DEBUG

			// Por cada hijo reviso listas y veo si agrego a OPEN
			while (!currNode->children.empty()) {
				bool isWorse = false;
				shared_ptr<Board> currChildNode = currNode->children.top(); currNode->children.pop();
				// Estadisticas
				Statistics::instance->astar_visited_nodes++;

				// Chequeo victoria
				if (currChildNode->isVictory()) {
					closed.push_back(currChildNode);
					// Estadisticas
					cout << "NODOS A*:" << Statistics::instance->astar_visited_nodes << endl;
					// Reconstruccion camino
					vector<shared_ptr<Board>> path = {};
					shared_ptr<Board> currPathNode = currChildNode;
					// Ciclo hijo->padre
					while (currPathNode->parent != nullptr) {
						path.push_back(currPathNode);
						currPathNode = currPathNode->parent;
					}
					// Meto raiz
					path.push_back(currPathNode);
					// Retorno camino
					return path;
				}

				// Comparar a OPEN
				auto openCopy = open;
				for (int j = 0; j < (int)open.size(); j++) {
					shared_ptr<Board> currOpenNode = openCopy.top(); openCopy.pop();
					if (isEqualPosition(currChildNode, currOpenNode) &&
						currChildNode->fscore <= currOpenNode->fscore) {
						isWorse = true; break;
					}
				}
				if (isWorse) { continue; }

				// Comparar a CLOSED
				for (int j = 0; j < (int)closed.size(); j++) {
					;
					if (isEqualPosition(currChildNode, closed[j]) &&
						currChildNode->fscore <= closed[j]->fscore) {
						isWorse = true; break;
					}
				}
				if (isWorse) { continue; }

				// No existe en OPEN ni CLOSED, o existe pero es MEJOR...
				open.push(currChildNode);
			}

			// Cierro el nodo
			closed.push_back(currNode);
		}

		// Si no encuentra el objetivo, retorna vacio
		return vector<shared_ptr<Board>>();
	}

	// Heuristica de numero de autos bloqueando
	int heuristicBlockingCars() {
		int rightmostIndex = exitIndex;
		int leftmostIndex = playerCar->getOriginIndex() - 1;
		int blockingCounter = 0;
		bitset<SIZE> checkable_positions = 0b0;
		for (int i = exitIndex; i <= leftmostIndex; i++) {
			checkable_positions[i] = 1;
		}
		// Conteo de autos bloqueando
		for (int i = 0; i < (int)cars.size(); i++) {
			if (cars[i]->id != 'R' && (cars[i]->position & checkable_positions).any()) {
				blockingCounter++;
			}
		}

		// Se devuelve negativo, pues mientras mas bloqueos, peor
		return -1 * blockingCounter;
	}

	// Chequea victoria
	bool isVictory() {
		for (int i = 0; i < (int)cars.size(); i++) {
			if (cars[i]->id == 'R' && cars[i]->position[exitIndex] == 1) {
				return true;
			}
			else {
				return false;
			}
		}
	}

	// Chequear si dos nodos tienen igual posicion de autos
	bool isEqualPosition(shared_ptr<Board> a, shared_ptr<Board> b) {
		if (a->board != b->board) return false; // Comp. Tablero general (1s y 0s)
		else if (a->cars.size() != b->cars.size()) return false; // Numero de autos
		// Comparacion pos. ENTRE autos
		int size = a->cars.size();
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++ ) {
				if (a->cars[i]->id == b->cars[j]->id &&
					a->cars[i]->position != b->cars[j]->position) {
					return false;
				}
			}
		}
		return true;
	}

	// Setear puzle por defecto
	void setDefaultPuzzle() {
		cars.clear();
		addPlayerCar(20, 2);
		addCar(33, 2, true);
		addCar(26, 2, false);
		addCar(30, 2, true);
		addCar(28, 2, true);
		addCar(19, 2, false);
		addCar(12, 3, false);
		addCar(10, 3, false);
		addCar(3, 2, false);
		addCar(6, 3, true);
	}

	// Puzle debugging
	void setDebugPuzzle() {
		cars.clear();
		addPlayerCar(22, 2);
		addCar(21, 2, false);
		addCar(20, 2, false);
		addCar(19, 2, false);
		addCar(18, 2, false);
		addCar(14, 2, true);
		addCar(32, 2, true);
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
		for (int i = SIZE - 1; i >= 0; i--) {
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