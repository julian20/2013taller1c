/*
 * GolemSpellItem.cpp
 *
 *  Created on: 11/06/2013
 *      Author: tomas
 */

#include <model/entities/Items/GolemSpellItem.h>
#include <model/entities/player/Player.h>

GolemSpellItem::GolemSpellItem() :
		Item() {
}

GolemSpellItem::GolemSpellItem(Item* entity) :
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

void GolemSpellItem::applyEffects(Player& entity) {
	Player* player = (Player*)&entity;

	player->enableGolem();
}

void GolemSpellItem::collideTo(Player& entity) {
	applyEffects(entity);
	removeFromGame = true;
}

string GolemSpellItem::getClassName() {
	return "GolemSpellItem";
}

GolemSpellItem::~GolemSpellItem() {
}
