/*
 * Entity.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#include <model/entities/Entity.h>

/**
 * TODO Player, personaje deben heredar de esta entidad, ya que
 * tendrian los mismos atributos basicos.
 */

Entity::Entity() {
	this->position = new Position(0, 0, 0);
	this->speed = new Speed(0, new Position(0, 0, 0));
	this->name = "";
}

Entity::Entity(std::string name, Position* position, Speed* speed) {
	this->position = position;
	this->speed = speed;
	this->name = name;
}

Entity::~Entity() {
	delete this->position;
	delete this->speed;
}

const Position* Entity::getPosition() const {
	return position;
}

void Entity::setPosition(Position* position) {
	this->position = position;
}

const Speed* Entity::getSpeed() const {
	return speed;
}

void Entity::setSpeed(Speed* speed) {
	this->speed = speed;
}

const std::string Entity::getName() const {
	return name;
}

void Entity::setName(std::string name) {
	this->name = name;
}
