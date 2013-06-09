/*
 * Item.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/Item.h>
#include <model/entities/player/Player.h>

Item::Item() : Entity() {
	this->attackable = false;
	this->walkable = true;
	this->hideInFog = true;
}

Item::Item(Item* entity) : Entity(entity) {
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
	this->attackable = false;
	this->walkable = true;
	this->hideInFog = true;
}

string Item::getClassName() {
	return "Item";
}

void Item::applyEffects(MobileEntity& entity) {
}

void Item::applyEffects(Entity& entity) {
}

void Item::collideTo(MobileEntity& mobileEntity) {
}

void Item::collideTo(Player& player) {
}

Item::~Item() {

}

