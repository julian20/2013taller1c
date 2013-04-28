/* 
 * File:   Position.cpp
 * Author: lucas
 * 
 * Created on March 21, 2013, 10:58 AM
 */

#include <model/entityProperties/Position.h>

Position::Position() {
	x = 0;
	y = 0;
	z = 0;
}

Position::Position(int coordX, int coordY, int coordZ) {
	x = coordX;
	y = coordY;
	z = coordZ;
}

Position::Position(int coordX, int coordY) {
	x = coordX;
	y = coordY;
	z = 0;
}

void Position::changeTo(int newX, int newY, int newZ) {
	x = newX;
	y = newY;
	z = newZ;
}

int Position::getX() const {
	return x;
}

void Position::setX(int x) {
	this->x = x;
}

int Position::getY() const {
	return y;
}

void Position::setY(int y) {
	this->y = y;
}

int Position::getZ() const {
	return z;
}

void Position::setZ(int z) {
	this->z = z;
}

Position& Position::operator=(Position &other){
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Position& pos){
	out << pos.x << " " << pos.y << " " << pos.z;
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Position& pos){
	int x,y,z;
	in >> x;
	in >> y;
	in >> z;
	pos.changeTo(x,y,z);
	return in;
}
