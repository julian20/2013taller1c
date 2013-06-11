/*
 * Flag.cpp
 *
 *  Created on: 06/06/2013
 *      Author: damian
 */

#include <model/entities/Flag.h>

Flag::Flag() {
	this->currentPos = new Vector3(0, 0, 0);
		this->currentTile = NULL;
		this->coord = new Coordinates(0, 0);
		this->base = new Base();
		this->life = 30;
		this->magic = 50;
		this->team = 0;
		this->killedBy = 0;
	//	this->positionInitialized = false;
		this->damageBuffer = 0;
		this->walkable = false;
		this->attackable = true;
		this->shield = 0;
		this->removeFromGame = false;
		this->hideInFog = false;
	player = "";
	name = "Flag";
}

void Flag::attackedBy(string player) {
	this->player = player;
}

string Flag::getClassName() {
	return this->name;
}

void Flag::collideTo(Player& player) {
	if (!isDead()) {
		receiveDamage(player);
		if (life <= 0) {
			killedBy = player.getTeam();
		}
	}
}

void Flag::receiveDamage(MobileEntity& player) {
	life -= player.getDamageBuffer();
}

Flag::~Flag() {
}

