/*
 * Entity.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#include <model/entities/Entity.h>
#include <model/map/Tile.h>
#include <model/entities/MobileEntity.h>

#include <vector>
#include <stdio.h>
#include <vector>

using namespace std;

Entity::Entity() {
	this->currentPos = new Vector3(0, 0, 0);
	this->currentTile = NULL;
	this->coord = new Coordinates(0,0);
	this->base = new Base();
	this->name = "";
	this->life = 100;
	this->team = 0;
	this->killedBy = 0;
}

Entity::Entity(Entity* entity) {
	Vector3* pos = entity->getCurrentPos();
	this->currentPos = new Vector3(pos->getX(),pos->getY(),pos->getZ());
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

void Entity::setPos(float x, float y, float z) {
	currentPos->setValues(x, y, z);
	coord->changeTo((int) x / Tile::computePositionTile(0, 0).w,
			(int) y / Tile::computePositionTile(0, 0).h);
}
bool Entity::isMobile()
{
	return this->mobile;
}
list<Entity*> Entity::getVisibleEnemies()
{
	return this->enemies;
}
Vector3* Entity::getCurrentPos() {
	return currentPos;
}

Coordinates Entity::getCoordinates() {
	Coordinates retval;
	retval.setRow(coord->getRow());
	retval.setCol(coord->getCol());

	return retval;
}

void Entity::setCoordinates(int row, int col) {
	if (row < 0 || col < 0) {
		std::cout << "Se esta tratando de asignar unas coordenadas"
					 " fuera de rango a una entity" << std::endl;
		return;
	}

	coord->changeTo(row, col);
}

string Entity::getClassName() {
	return "Entity";
}

Base* Entity::getBase() {
	return this->base;
}

void Entity::setBase(Base* base) {
	this->base = base;
}

void Entity::setBaseSizes(int _rows, int _cols) {
	this->base->setRows(_rows);
	this->base->setCols(_cols);
}

std::string Entity::getName() {
	return this->name;
}

void Entity::setName(std::string name) {

	this->name = name;
}

int Entity::getLife() {
	return this->life;
}

void Entity::setTile(Tile* _tile) {
	if (this->currentTile)
		delete this->currentTile;
	currentTile = _tile;

	// Las coordinates se actualizan en EntityViewMap
}

Tile* Entity::getTile() {
	// Devuelve una copia del tile
	Tile* retval = new Tile(new Coordinates(coord->getRow(),coord->getCol()));

	return retval;
}

void Entity::applyDamage(int damage)
{
	//int totalDamage=damage falta calcular lo q protege el escudo
	this->life= this->life-damage;

}


bool Entity::isDead() {
	if (this->life <= 0) {
		return true;
	} else {
		return false;
	}
}

int Entity::getTeam() {
	return this->team;
}

int Entity::getKilledBy() {
	return this->killedBy;
}

void Entity::setTeam(int team) {
	this->team = team;
}

void Entity::setKilledBy(int team) {
	this->killedBy = team;
}

void Entity::collideTo(MobileEntity* mobileEntity) {
	mobileEntity->attack(this);
}

Entity::~Entity() {
	delete this->coord;
	delete this->currentPos;
	delete this->base;
}
