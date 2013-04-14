/*
 * Coordinates.cpp
 *
 *  Created on: Apr 2, 2013
 *      Author: fede
 */

#include <model/entityProperties/Coordinates.h>

Coordinates::Coordinates() {
	changeTo(0, 0);
}

Coordinates::Coordinates(int _row, int _col) {
	changeTo(_row, _col);
}

void Coordinates::changeTo(int _row, int _col) {
	row = _row;
	col = _col;
}

int Coordinates::getRow() const {
	return row;
}

void Coordinates::setRow(int _row){
	row = _row;
}

int Coordinates::getCol() const {
	return col;
}

void Coordinates::setCol(int _col){
	col = _col;
}
