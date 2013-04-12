/*
 * Tile.cpp
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#include <model/map/Tile.h>
#include <model/entityProperties/Position.h>
#include <string>

#define TilesOverlap	2		// px
#define TextureWidth	70		// px
#define TextureHeight	50		// px

Tile::Tile(Position* position, std::string textureId) {
	this->coordinates = new Coordinates(0, 0);
	this->position = position;
	this->textureIdentifier = textureId;
}

Tile::Tile(Coordinates* _coordinates) {
	this->coordinates = _coordinates;
	this->textureIdentifier = "";

	SDL_Rect pos = this->computePositionTile(coordinates->getRow(), coordinates->getCol());
	this->position = new Position(pos.x, pos.y);
}

Tile::Tile() {
	this->position = new Position(0, 0, 0);
	this->coordinates = new Coordinates(0, 0);
}

Tile::~Tile() {
	delete this->position;
	if (coordinates)
			delete coordinates;
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

Position Tile::computePosition(int row, int col /*, toTileCenter = false*/) {
	return getDiamondShapeMapPos(row, col);
}

Position Tile::computePosition(int row, int col, bool toTileCenter) {
	Position pos = computePosition(row, col);

	if (toTileCenter) {
		pos.setX(pos.getX() + TextureWidth/2);
		pos.setY(pos.getY() + TextureHeight/2);
	}

	return pos;
}

SDL_Rect Tile::computePositionTile(int row, int col /*, toTileCenter = false*/) {
	return getDiamondShapeMapTilePos(row, col);
}

SDL_Rect Tile::computePositionTile(int row, int col, bool toTileCenter) {
	SDL_Rect pos = computePositionTile(row, col);

	if (toTileCenter) {
		pos.x = pos.x + TextureWidth/2;
		pos.y = pos.y + TextureHeight/2;
	}

	return pos;
}

SDL_Rect Tile::getSquaredMapTilePos(int row, int col) {
	int widthTexture = TextureWidth - TilesOverlap;
	int heightTexture = TextureHeight - TilesOverlap;

	SDL_Rect posTile;

	posTile.x = (row % 2) * widthTexture / 2
			+ col * widthTexture;
	posTile.y = row * heightTexture / 2;
	posTile.w = widthTexture;
	posTile.h = heightTexture;

	return posTile;
}

SDL_Rect Tile::getDiamondShapeMapTilePos(int row, int col) {
	int widthTexture = TextureWidth - TilesOverlap;
	int heightTexture = TextureHeight - TilesOverlap;

	Position pos = getDiamondShapeMapPos(row, col);
	SDL_Rect posTile;

	posTile.x = pos.getX();
	posTile.y = pos.getY();
	posTile.w = widthTexture;
	posTile.h = heightTexture;

	return posTile;
}

Position Tile::getDiamondShapeMapPos(int row, int col) {
	int widthTexture = TextureWidth - TilesOverlap;
	int heightTexture = TextureHeight - TilesOverlap;

	Position pos;

	pos.setX((col - row) * widthTexture / 2);
	pos.setY((col + row) * heightTexture / 2);

	return pos;
}

Coordinates* Tile::getTileCoordinates(int x, int y)
{
	float xF = (float)x;
	float yF = (float)y;

	float widthTexture = (float)TextureWidth - (float)TilesOverlap;
	float heightTexture = (float)TextureHeight - (float)TilesOverlap;

	int col = (int)(roundf(xF / widthTexture + yF / heightTexture));
	int row = (int)(roundf(yF / heightTexture - xF / widthTexture));

	// printf("x: %i   Y: %i\n",x,y);

	Coordinates* coor = new Coordinates(row,col);
	return coor;
}
