/*
 * TileData.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: lucas
 */

#include <model/map/TileData.h>
#include <stdio.h>
namespace std {

TileData::TileData() {
	personaje = NULL;
	//TODO: cambiar tileType de int a string
	tileType = 0;
	iterator = entities.begin();
}

int TileData::getType(){
	return tileType;
}

Personaje* TileData::getPersonaje(){
	return personaje;
}

Entity* TileData::getNextEntity(){
	if (iterator== entities.end() || getNumberOfEntitiesOnTile()<=0)
		return NULL;
	Entity* entity = *iterator;
	++iterator;
	return entity;
}


int TileData::getNumberOfEntitiesOnTile(){
	return entities.size();
}

void TileData::setType(int type){
	tileType=type;
}

void TileData::setPersonaje(Personaje* newPersonaje){
	personaje=newPersonaje;
}

void TileData::addEntity(Entity* entity){
	//Se agrega la entidad al final del vector.
	entities.push_back(entity);
}


TileData::~TileData() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
