/*
 * Speed.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#include <model/entityProperties/Speed.h>
#include <model/entityProperties/Position.h>
#include <model/Vector2.h>

Speed::Speed() {
	this->magnitude = 0;
	this->direction = new Vector2(0, 0);
}

Speed::Speed(int magnitude, Vector2* direction) {
	this->magnitude = magnitude;
	this->direction = direction;
}

Speed::~Speed() {
}

int Speed::getMagnitude() {
	return magnitude;
}

void Speed::setMagnitude(int magnitude) {
	this->magnitude = magnitude;
}

Vector2* Speed::getDirection() {
	return direction;
}

void Speed::setDirection(Vector2* direction) {
	this->direction->setValues(direction->getX(),direction->getY());
}

Speed& Speed::operator=(Speed &other) {
	this->magnitude = other.magnitude;
	*(this->direction) = *(other.direction);
	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Speed& speed) {
	out << speed.magnitude << " " << *(speed.direction);
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Speed& speed) {
	int mag;
	Vector2* vect = new Vector2(0, 0);
	in >> mag;
	in >> *vect;
	speed.setMagnitude(mag);
	speed.setDirection(vect);
	return in;
}
