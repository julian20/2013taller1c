/*
 * FrostWandWeapon.cpp
 *
 *  Created on: 11/06/2013
 *      Author: champion
 */

#include <model/Weapons/FrostWandWeapon.h>

FrostWandWeapon::FrostWandWeapon() : Weapon() {
	this->accuracy = 10;
	this->damage = 10;
	this->magic = 0;
	this->range = 1;
	this->name = "FrostWand";
}

void FrostWandWeapon::attack(Entity& entity) {
	if (entity.getClassName() == "Player" ||
		entity.getClassName() == "MobileEntity") {

		MobileEntity* mobile = (MobileEntity*)&entity;

		mobile->froze();
	}
}

FrostWandWeapon::~FrostWandWeapon() {

}

