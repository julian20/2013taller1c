/*
 * SpellEffect.cpp
 *
 *  Created on: Jun 5, 2013
 *      Author: mint
 */

#include <model/entities/spells/SpellEffect.h>
#include <model/map/MapData.h>

SpellEffect::SpellEffect() {
	this->maxTraveledTiles = 0;
	this->speed->setMagnitude(6);
	this->currentTile = new Tile(new Coordinates(0, 0));
	this->walkable = true;

	defineConstants();

	this->direction = LEFT;
}

SpellEffect::SpellEffect(SpellEffect* spell) {
	this->maxTraveledTiles = spell->getMaxTraveledTiles();
	this->speed= spell->getSpeed();
	this->currentTile = new Tile(new Coordinates(0, 0));
	this->walkable = true;

	defineConstants();

	Vector3 dir = spell->getDirection();
	this->direction.setValues(dir.getX(), dir.getY());
}

void SpellEffect::defineConstants() {
	LEFT.setValues(-1, 0);
	UP_LEFT.setValues(-1, -1);
	UP.setValues(0, -1);
	UP_RIGHT.setValues(1, -1);
	RIGHT.setValues(1, 0);
	DOWN_RIGHT.setValues(1, 1);
	DOWN.setValues(0, 1);
	DOWN_LEFT.setValues(-1, 1);
}

void SpellEffect::applyEffects(Entity& entity){

}

void SpellEffect::applyEffects(MobileEntity& entity){

}

void SpellEffect::collideTo(MobileEntity& entity){

}

bool SpellEffect::reachMaxDistance() {
	Tile* startingTile = new Tile(startingCoords);
	int distanceTraveled = MapData::distBetweenTilesInTiles(startingTile, currentTile);
	delete startingTile;

	return (maxTraveledTiles >= distanceTraveled);
}

void SpellEffect::update(MapData* mapData) {
	if (reachMaxDistance())
		return;

	currentPos->add(&direction);
}

// Se debe llamar luego de seter las coordenadas y la direccion
void SpellEffect::setUp() {
	// Seteo la posicion del personaje en la pantalla
	Position* posTile = Tile::computePosition(coord->getRow(), coord->getCol());
	setPos((float) posTile->getX(), (float) posTile->getY());
	delete posTile;

	// Calculo el vector direccion a sumar en cada update
	float relationSpeed = ((float) Tile::getTileHeight())
			/ ((float) Tile::getTileWidth());

	direction.normalize();
	direction.multiplyBy(fabs(direction.getY()) * (relationSpeed - 1) + 1);
	direction.multiplyBy(getSpeed()->getMagnitude());

	// Seteo la coordenada inicial para fijarme cuantos tiles ya recorri
	startingCoords.changeTo(coord->getRow(), coord->getCol());
}

string SpellEffect::getClassName(){
	return string("SpellEffect");
}

void SpellEffect::setDirection(Vector3 direction) {
	this->direction = direction;
}

Vector3 SpellEffect::getDirection(){
	return direction;
}

void SpellEffect::setMaxTraveledTiles(int maxTraveledTiles) {
	this->maxTraveledTiles = maxTraveledTiles;
}

int SpellEffect::getMaxTraveledTiles(){
	return maxTraveledTiles;
}

SpellEffect::~SpellEffect() {

}

// Constantes

Vector3 SpellEffect::getLEFT() {
	return LEFT;
}

Vector3 SpellEffect::getUP_LEFT() {
	return UP_LEFT;
}

Vector3 SpellEffect::getUP() {
	return UP;
}

Vector3 SpellEffect::getUP_RIGHT() {
	return UP_RIGHT;
}

Vector3 SpellEffect::getRIGHT() {
	return RIGHT;
}

Vector3 SpellEffect::getDOWN_RIGHT() {
	return DOWN_RIGHT;
}

Vector3 SpellEffect::getDOWN() {
	return DOWN;
}

Vector3 SpellEffect::getDOWN_LEFT() {
	return DOWN_LEFT;
}
