/*
 * Tile.cpp
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#include <model/map/Tile.h>
#include <model/entityProperties/Position.h>
#include <string>

Tile::Tile(Position* position, std::string textureId) {
	this->coordinates = new Coordinates(0, 0);
	this->position = position;
	this->textureIdentifier = textureId;
}

Tile::Tile() {
	this->position = new Position(0, 0, 0);
	this->coordinates = new Coordinates(0, 0);
}

Tile::~Tile() {
	delete this->position;
}

Position* Tile::getPosition() {
	return this->position;
}

void Tile::setPosition(int x, int y, int z) {
	this->position->changeTo(x, y, z);
}

void Tile::setPosition(Position* position) {
	this->position = position;
}

Coordinates* Tile::getCoordinates() {
	return this->coordinates;
}

void Tile::setCoordinates(int _row, int _col) {
	this->coordinates->changeTo(_row, _col);
}

std::string Tile::getTextureIdentifier() {
	return this->textureIdentifier;
}

void Tile::setTextureIdentifier(std::string textureId) {
	this->textureIdentifier = textureId;
}

