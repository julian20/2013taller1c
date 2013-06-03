/*
 * LifeHeart.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/LifeHeart.h>

LifeHeart::LifeHeart() {
}

LifeHeart::LifeHeart(Item* entity) {
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

void LifeHeart::applyEffects(Entity& entity) {
	// TODO: algun atributo o define de vida original.
	entity.setLife(100);
}

void LifeHeart::applyEffects(MobileEntity& entity) {
	entity.setLife(0);
}

void LifeHeart::collideTo(MobileEntity& entity) {
	applyEffects(entity);
}

string LifeHeart::getClassName() {
	return "LifeHeart";
}

LifeHeart::~LifeHeart() {
}

