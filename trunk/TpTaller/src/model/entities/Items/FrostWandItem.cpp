/*
 * FrostWandItem.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/FrostWandItem.h>

FrostWandItem::FrostWandItem() {
}

FrostWandItem::FrostWandItem(Item* entity) : Item(entity) {
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

void FrostWandItem::applyEffects(Player& entity) {
	// TODO: agregar frost wand.
}

void FrostWandItem::collideTo(Player& entity) {
	applyEffects(entity);
	removeFromGame = true;
}

string FrostWandItem::getClassName() {
	return "FrostWandItem";
}

FrostWandItem::~FrostWandItem() {
}

