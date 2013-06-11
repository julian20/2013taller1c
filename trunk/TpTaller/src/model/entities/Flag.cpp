/*
 * Flag.cpp
 *
 *  Created on: 06/06/2013
 *      Author: damian
 */

#include <model/entities/Flag.h>

Flag::Flag() {
	player = "";
	classname = "Flag";
	life = 30;
	attackable = true;
}

Flag::Flag(Entity* entity) :
		Entity(entity) {
	Vector3* pos = entity->getCurrentPos();
	this->currentPos = new Vector3(pos->getX(), pos->getY(), pos->getZ());
	Coordinates coordin = entity->getCoordinates();
	this->coord = new Coordinates(coordin.getCol(), coordin.getRow());
	Base base = entity->getBase();
	Base* newBase = new Base(base);
	this->base = newBase;
	this->name = entity->getName();
	this->currentTile = NULL;
	this->team = entity->getTeam();
	this->killedBy = entity->getKilledBy();
	this->damageBuffer = 0;
	this->magic = entity->getMagic();
	this->walkable = entity->isWalkable();
	this->attackable = entity->isAttackable();
	this->hideInFog = entity->getHideInFog();
	this->shield = 0;
	this->removeFromGame = false;

	player = "";
	classname = "Flag";
	life = 30;
	attackable = true;
}

void Flag::attackedBy(string player) {
	this->player = player;
}

string Flag::getClassName() {
	return this->classname;
}

void Flag::collideTo(Player& player) {
	if (!isDead()) {
		receiveDamage(player);
		if (life <= 0) {
			killedBy = player.getTeam();
		}
	}
}

void Flag::receiveDamage(Player& player) {
	player.attack(*this);
	cout << this->life << endl;
}

Flag::~Flag() {
}

