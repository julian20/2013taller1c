/*
 * Chest.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/Chest.h>
#include <model/map/MapData.h>

Chest::Chest() : Item() {
	this->containingItem = NULL;
	this->life = 1;
	this->walkable = false;
	this->attackable = true;
}

Chest::Chest(Item* entity) : Item(entity) {
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
	this->containingItem = NULL;
	this->walkable = false;
	this->attackable = true;
}

void Chest::setItemToContaing(Item* containingItem) {
	this->containingItem = containingItem;
}

void Chest::applyEffects(Player& entity) {
	entity.attack(*this);
}

void Chest::collideTo(Player& entity) {
	applyEffects(entity);
}

void Chest::onRemove(MapData* mapData) {
	if (this->containingItem == NULL)  {
		std::cout << "No se ha asignado item al chest" << std::endl;
		return;
	}

	mapData->addItem(coord->getRow(), coord->getCol(), containingItem);
}

string Chest::getClassName() {
	return "Chest";
}

Chest::~Chest() {
}

