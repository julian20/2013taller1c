/*
 * Golem.cpp
 *
 *  Created on: 09/06/2013
 *      Author: damian
 */

#include <model/entities/Golem/Golem.h>
#define GOLEM_COST 50
Golem::Golem() {
	this->life = 200 ;
	this->magic = 0;
	this->name = "Golem";
}

int Golem::cost(){
	return GOLEM_COST;
}

void Golem::setOwner(Player* owner){
	this->owner = owner;
}

Player* Golem::getOwner(){
	return owner;
}

Golem::~Golem() {
	// TODO Auto-generated destructor stub
}

