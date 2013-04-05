/*
 * Entity.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#include <model/entities/Entity.h>

#include <vector>
#include <stdio.h>
/**
 * TODO Player, personaje deben heredar de esta entidad, ya que
 * tendrian los mismos atributos basicos.
 */

/*
 * Entity.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#include <model/entities/Entity.h>

#include <vector>

/**
 * TODO Player, personaje deben heredar de esta entidad, ya que
 * tendrian los mismos atributos basicos.
 */
namespace std {
	Entity::Entity() {
		this->currentPos = new Vector2(0, 0);
		this->base = new Base();
	}

	void Entity::setPos(float x, float y) {
		currentPos->SetValues(x, y);
	}

	string Entity::getClassName(){
		return "Entity";
	}

	Base* Entity::getBase() {
		return this->base;
	}

	void Entity::setBase(Base* base) {
		this->base = base;
	}

	Entity::~Entity() {
		delete this->currentPos;
		delete this->base;
	}
}
