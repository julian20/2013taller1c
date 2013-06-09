/*
 * Golem.cpp
 *
 *  Created on: 09/06/2013
 *      Author: damian
 */

#include <model/entities/Golem/Golem.h>
#define GOLEM_COST 20
Golem::Golem() {
	this->life = 200 ;
	this->magic = 0;
	this->name = "Golem";
}
int Golem::cost()
{
	return GOLEM_COST;
}

Golem::~Golem() {
	// TODO Auto-generated destructor stub
}

