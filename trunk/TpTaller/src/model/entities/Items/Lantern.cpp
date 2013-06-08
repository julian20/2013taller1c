/*
 * Lantern.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: gonchub
 */

#include <model/entities/Items/Lantern.h>

Lantern::Lantern() {
}

Lantern::Lantern(Item* entity) : Item(entity) {
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

// TODO: como el manejo de colisiones esta en mobile entity,
// el player cuando colisiona con otra cosa nunca sabe que
// es de tipo player, o al menos no pude conseguir su tipo
// bien. Una solucion seria pasar el atributo view range
// a mobile entity, pero sino nose.
void Lantern::applyEffects(Player& entity) {
	entity.setViewRange(entity.getViewRange() + 50);
}

void Lantern::collideTo(Player& entity) {
	applyEffects(entity);
}

string Lantern::getClassName() {
	return "Lantern";
}

Lantern::~Lantern() {
}

