/*
 * Flag.cpp
 *
 *  Created on: 06/06/2013
 *      Author: damian
 */

#include <model/entities/Flag.h>

Flag::Flag() {
	player="";
	name="Flag";
}
void Flag::attackedBy(string player)
{
	this->player=player;
}
string Flag::getClassName()
{
	return this->name;
}
Flag::~Flag() {
}

