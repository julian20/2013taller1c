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
	this->direction = Vector2(0, 0);
}

Speed::Speed(int magnitude, Vector2 direction) {
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

Vector2 Speed::getDirection() {
	return direction;
}

void Speed::setDirection(Vector2 direction) {
	this->direction = direction;
}
