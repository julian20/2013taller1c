/*
 * MagicBottle.cpp
 *
 *  Created on: 02/06/2013
 *      Author: damian
 */

#include <model/entities/Items/MagicBottle.h>

MagicBottle::MagicBottle() {
}

MagicBottle::MagicBottle(Item* entity) {
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

void MagicBottle::applyEffects(Player& entity) {
	entity.setMagic(100);
}

void MagicBottle::collideTo(Player& entity) {
	applyEffects(entity);
	removeFromGame = true;
}

string MagicBottle::getClassName() {
	return "MagicBottle";
}

MagicBottle::~MagicBottle() {
}

