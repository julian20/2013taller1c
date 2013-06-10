/*
 * SpeedBoots.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/SpeedBoots.h>

SpeedBoots::SpeedBoots() {
}

SpeedBoots::SpeedBoots(Item* entity) :
		Item(entity) {
	Vector3* pos = entity->getCurrentPos();
	this->currentPos = new Vector3(pos->getX(), pos->getY(), pos->getZ());
	Coordinates coordin = entity->getCoordinates();
	this->coord = new Coordinates(coordin.getCol(), coordin.getRow());
	Base base = entity->getBase();
	Base* newBase = new Base(base);
	this->base = newBase;
	this->name = entity->getName();
	this->life = entity->getLife();
	this->currentTile = NULL;
	this->team = entity->getTeam();
	this->killedBy = entity->getKilledBy();
}

void SpeedBoots::applyEffects(Player& entity) {
	entity.setSpeedMagnitude(entity.getSpeed()->getMagnitude() + 1);
	entity.setInitSpeedMagnitude(entity.getInitSpeed()->getMagnitude() + 1);
}

void SpeedBoots::collideTo(Player& entity) {
	applyEffects(entity);
	removeFromGame = true;
}

string SpeedBoots::getClassName() {
	return "SpeedBoots";
}

SpeedBoots::~SpeedBoots() {
}

