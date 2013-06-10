/*
 * LifeHeart.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/LifeHeart.h>
#include <model/entities/player/Player.h>

LifeHeart::LifeHeart() : Item() {
}

LifeHeart::LifeHeart(Item* entity) : Item(entity) {
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

void LifeHeart::applyEffects(Player& entity) {
	entity.setLife(100);
}

void LifeHeart::collideTo(Player& entity) {
	applyEffects(entity);
	removeFromGame = true;
}

string LifeHeart::getClassName() {
	return "LifeHeart";
}

LifeHeart::~LifeHeart() {
}

