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

Entity::Entity() {
	currentPos = new Vector2(0, 0);
}



void Entity::setPos(float x, float y) {
	printf("Asd\n");
	 currentPos->SetValues(x, y);
}

Entity::~Entity(){
}
