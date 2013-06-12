/*
 * Sword.cpp
 *
 *  Created on: 01/06/2013
 *      Author: damian
 */

#include <model/Weapons/Sword.h>

Sword::Sword() : Weapon() {
	this->accuracy = 10;
	this->damage = 10;
	this->magic = 0;
	this->range = 1;
	this->name = "Sword";
}

Sword::~Sword() {

}

