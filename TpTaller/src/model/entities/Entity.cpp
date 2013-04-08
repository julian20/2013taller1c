/*
 * Entity.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#include <model/entities/Entity.h>
#include <model/entities/Entity.h>
#include <model/map/Tile.h>

#include <vector>
#include <stdio.h>
#include <vector>

using namespace std;

Entity::Entity() {
	this->currentPos = new Vector2(0, 0);
	this->base = new Base();
	this->name = "";
}

void Entity::setPos(float x, float y) {
	currentPos->SetValues(x,y);
}

Vector2* Entity::getCurrentPos() {
	return currentPos;
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

std::string Entity::getName() {
	return this->name;
}

void Entity::setName(std::string name) {
	this->name = name;
}

Entity::~Entity() {
	delete this->currentPos;
	delete this->base;
}

