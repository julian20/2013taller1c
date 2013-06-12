/*
 * Golem.cpp
 *
 *  Created on: 09/06/2013
 *      Author: damian
 */

#include <model/entities/Golem/Golem.h>

Golem::Golem() {
	this->life = 10 ;
	this->magic = 0;
	this->name = "Golem";
	this->owner = NULL;
	attackTimer.start();
	this->id = 0;
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
	if (entityPtr->getName() == "flago") return;
	entityPtr->applyDamage(2);
	if(entityPtr->isDead()) entityPtr->setKilledBy(team);
}

void Golem::setId(int id){
	this->id = id;
}

int Golem::getId(){
	return id;
}

void Golem::attack(Entity& entity) {
	Entity* entityPtr = (Entity*) &entity;
	if (entityPtr->getName() == "flago") return;
	if (attackTimer.getTimeIntervalSinceStart() > ATTACK_TIMEOUT) {
		entityPtr->setLife(entityPtr->getLife() - 2);
		attackTimer.start();
	}
	if(entityPtr->isDead()) entityPtr->setKilledBy(team);
}

Golem::~Golem() {
}

