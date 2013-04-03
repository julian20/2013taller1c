/*
 * Tile.cpp
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#include <model/map/Tile.h>
#include <model/entityProperties/Position.h>
#include <string>

#define MapMargin		40		// px
#define TilesOverlap	2		// px
#define TextureWidth	70		// px
#define TextureHeight	50		// px

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

SDL_Rect Tile::computePosition(int row, int col) {
	return getSquaredMapTilePos(row, col);
}

SDL_Rect Tile::getSquaredMapTilePos(int row, int col) {
	int widthTexture = TextureWidth - TilesOverlap;
	int heightTexture = TextureHeight - TilesOverlap;

	SDL_Rect posTile;

	posTile.x = MapMargin + (row % 2) * widthTexture / 2
			+ col * widthTexture;
	posTile.y = MapMargin + row * heightTexture / 2;
	posTile.w = widthTexture;
	posTile.h = heightTexture;

	return posTile;
}

SDL_Rect Tile::getDiamondShapeMapTilePos(int row, int col) {
	int widthTexture = TextureWidth - TilesOverlap;
	int heightTexture = TextureHeight - TilesOverlap;

	SDL_Rect posTile;

	posTile.x = (col + row) * widthTexture / 2;
	posTile.y = (col - row) * heightTexture / 2;
	posTile.w = widthTexture;
	posTile.h = heightTexture;

	return posTile;
}

