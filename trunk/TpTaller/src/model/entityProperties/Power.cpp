/*
 * Power.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#include <model/entityProperties/Power.h>

#include <string>

Power::Power() {
	this->name = "";
	this->range = 0;
	this->damage = 0;
}

Power::Power(std::string name, int damage, int range) {
	this->name = name;
	this->range = range;
	this->damage = damage;
}

Power::~Power() {
}

int Power::getDamage() const {
	return damage;
}

void Power::setDamage(int damage) {
	this->damage = damage;
}

std::string Power::getName() {
	return name;
}

void Power::setName(std::string name) {
	this->name = name;
}

int Power::getRange() {
	return range;
}

void Power::setRange(int range) {
	this->range = range;
}
