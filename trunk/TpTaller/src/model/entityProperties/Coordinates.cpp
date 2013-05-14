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

Coordinates::Coordinates(const Coordinates &coords) {
	changeTo(coords.getRow(), coords.getCol());
}

void Coordinates::changeTo(int _row, int _col) {
	row = _row;
	col = _col;
}

int Coordinates::getRow() const {
	return row;
}

void Coordinates::setRow(int _row) {
	row = _row;
}

int Coordinates::getCol() const {
	return col;
}

void Coordinates::setCol(int _col) {
	col = _col;
}

bool Coordinates::isEqual(Coordinates coords){
	return ( (coords.getCol() == this->col) && (coords.getRow() == this->row) );
}

Coordinates& Coordinates::operator=(Coordinates &other) {
	this->col = other.col;
	this->row = other.row;

	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Coordinates& coor) {
	out << coor.row << " " << coor.col;
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Coordinates& coor) {
	int col, row;
	in >> row;
	in >> col;
	coor.changeTo(row, col);
	return in;
}
