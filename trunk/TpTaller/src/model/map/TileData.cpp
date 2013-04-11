/*
 * TileData.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: lucas
 */

#include <model/map/TileData.h>
#include <stdio.h>

using namespace std;

TileData::TileData() {
	personaje = NULL;
	//TODO: cambiar tileType de int a string
	tileType = "neutral";
	iterator = entities.begin();
}

std::string TileData::getType(){
	return tileType;
}

Player* TileData::getPersonaje(){
	return personaje;
}

Entity* TileData::getNextEntity(){
	if (iterator == entities.end() || getNumberOfEntitiesOnTile()<=0)
		return NULL;
	Entity* entity = *iterator;
	++iterator;
	return entity;
}


int TileData::getNumberOfEntitiesOnTile(){
	return entities.size();
}

void TileData::setType(std::string type){
	tileType = type;
}

void TileData::setPersonaje(Player* newPersonaje){
	personaje = newPersonaje;
}

void TileData::addEntity(Entity* entity){
	//Se agrega la entidad al final del vector.
	entities.push_back(entity);
}


TileData::~TileData() {
	// TODO Auto-generated destructor stub
}
