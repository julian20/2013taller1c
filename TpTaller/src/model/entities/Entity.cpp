/*
 * Entity.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#include <model/entities/Entity.h>
#include <model/map/Tile.h>

#include <vector>
#include <stdio.h>
#include <vector>

using namespace std;

Entity::Entity() {
	this->currentPos = new Vector3(0, 0, 0);
	this->coord = new Coordinates(0, 0);
	this->base = new Base();
	this->name = "";
	this->life = 100;
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

Entity::~Entity() {
	delete this->coord;
	delete this->currentPos;
	delete this->base;
}
