/*
 * SpellShieldItem.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/SpellShieldItem.h>

SpellShieldItem::SpellShieldItem() {
}

SpellShieldItem::SpellShieldItem(Item* entity) : Item(entity) {
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

void SpellShieldItem::applyEffects(Player& entity) {
	Player* player = (Player*)&entity;

	player->addShieldSpell();
}

void SpellShieldItem::collideTo(Player& entity) {
	applyEffects(entity);
}

string SpellShieldItem::getClassName() {
	return "SpellShieldItem";
}

SpellShieldItem::~SpellShieldItem() {
}

