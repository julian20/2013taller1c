/*
 * Speed.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#include <model/entityProperties/speed/Speed.h>
#include <model/entityProperties/position/Position.h>

Speed::Speed() {
	this->magnitude = 0;
	this->direction = new Position(0, 0, 0);
}

Speed::Speed(int magnitude, Position* direction) {
	this->magnitude = magnitude;
	this->direction = direction;
}

Speed::~Speed() {
	delete this->direction;
}

int Speed::getMagnitude() const {
	return magnitude;
}

void Speed::setMagnitude(int magnitude) {
	this->magnitude = magnitude;
}

const Position* Speed::getDirection() const {
	return direction;
}

void Speed::setDirection(Position* direction) {
	this->direction = direction;
}
