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
	this->position = position;
	this->textureIdentifier = textureId;
}

Tile::Tile() {
	this->position = new Position(0, 0, 0);
}

Tile::~Tile() {
	delete this->position;
}

Position* Tile::getPosition() {
	return this->position;
}

void Tile::setPosition(Position* position) {
	this->position = position;
}

std::string Tile::getTextureIdentifier() {
	return this->textureIdentifier;
}

void Tile::setTextureIdentifier(std::string textureId) {
	this->textureIdentifier = textureId;
}

