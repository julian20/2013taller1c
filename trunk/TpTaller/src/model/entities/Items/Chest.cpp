/*
 * Chest.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/Chest.h>

Chest::Chest() {
}

Chest::Chest(Item* entity) : Item(entity) {
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

void Chest::applyEffects(Player& entity) {
}

void Chest::collideTo(Player& entity) {
	applyEffects(entity);
}

string Chest::getClassName() {
	return "Chest";
}

Chest::~Chest() {
}

