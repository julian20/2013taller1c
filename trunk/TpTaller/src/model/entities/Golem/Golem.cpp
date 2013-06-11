/*
 * Golem.cpp
 *
 *  Created on: 09/06/2013
 *      Author: damian
 */

#include <model/entities/Golem/Golem.h>

Golem::Golem() {
	this->life = 200 ;
	this->magic = 0;
	this->name = "Golem";
}

int Golem::cost(){
	return GOLEM_COST;
}

void Golem::setOwner(Player* owner){
	this->owner = owner;
}

Player* Golem::getOwner(){
	return owner;
}

void Golem::collideTo(MobileEntity& entity) {
	// Casteo a puntero.
	MobileEntity* entityPtr = (MobileEntity*) &entity;
	entityPtr->applyDamage(10);
}

void Golem::attack(Entity& entity) {
	Entity* entityPtr = (Entity*) &entity;
	entityPtr->setLife(entityPtr->getLife() - 10);
}

Golem::~Golem() {
}

