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

unsigned int Tile::tileWidth = TextureWidth;
unsigned int Tile::tileHeight = TextureHeight;

Tile::Tile(Position* position, std::string textureId) {
	this->coordinates = new Coordinates(0, 0);
	this->position = position;
	this->textureIdentifier = textureId;
	this->fScore = 0;
}

Tile::Tile(Coordinates* _coordinates) {
	this->coordinates = _coordinates;
	this->textureIdentifier = "";
	this->position = new Position(0, 0, 0);
	this->fScore = 0;
	updatePosition();
}

Tile::Tile(Coordinates _coordinates) {
	this->coordinates = new Coordinates(0, 0);
	this->textureIdentifier = "";
	this->position = new Position(0, 0, 0);
	this->fScore = 0;
	setCoordinates(_coordinates);
}

Tile::Tile() {
	fScore = 0;
	this->position = new Position(0, 0, 0);
	this->coordinates = new Coordinates(0, 0);
}

Tile::~Tile() {
	delete this->position;
	if (coordinates)
			delete coordinates;
}

void Tile::updatePosition() {
	SDL_Rect pos = this->computePositionTile(coordinates->getRow(),
											 coordinates->getCol());
	setPosition(pos.x, pos.y);
}

Position* Tile::getPosition() {
	return this->position;
}

void Tile::setPosition(int x, int y) {
	this->position->changeTo(x, y);
}

void Tile::setPosition(int x, int y, int z) {
	this->position->changeTo(x, y, z);
}

void Tile::setPosition(Position* position) {
	this->position->changeTo(position->getX(),position->getY(), position->getZ());
}

Coordinates Tile::getCoordinates() {
	Coordinates retval;
	retval.setRow( coordinates->getRow() );
	retval.setCol( coordinates->getCol() );

	return retval;
}

void Tile::setCoordinates(int _row, int _col) {
	this->coordinates->changeTo(_row, _col);
	updatePosition();
}

void Tile::setCoordinates(Coordinates coords) {
	this->coordinates->changeTo(coords.getRow(), coords.getCol());
	updatePosition();
}

std::string Tile::getTextureIdentifier() {
	return this->textureIdentifier;
}

void Tile::setTextureIdentifier(std::string textureId) {
	this->textureIdentifier.assign(textureId);
}

Position* Tile::computePosition(int row, int col, bool toTileZero){
	Position* pos = getDiamondShapeMapPos(row, col);

	if (toTileZero) {
		const int widthTexture = tileWidth - TilesOverlap;
		const int heightTexture = tileHeight - TilesOverlap;

		pos->setX(pos->getX() + widthTexture/2);
		pos->setY(pos->getY() + heightTexture/2);
	}

	return pos;
}

SDL_Rect Tile::computePositionTile(int row, int col, bool toTileZero) {
	SDL_Rect rect = getDiamondShapeMapTilePos(row, col);

	if (toTileZero) {
		const int widthTexture = tileWidth - TilesOverlap;
		const int heightTexture = tileHeight - TilesOverlap;

		rect.x = rect.x + widthTexture/2;
		rect.y = rect.y + heightTexture/2;
	}

	return rect;
}

SDL_Rect Tile::getSquaredMapTilePos(int row, int col) {
	const int widthTexture = tileWidth - TilesOverlap;
	const int heightTexture = tileHeight - TilesOverlap;

	SDL_Rect posTile;

	posTile.x = (row % 2) * widthTexture / 2 + col * widthTexture;
	posTile.y = row * heightTexture / 2;
	posTile.w = widthTexture;
	posTile.h = heightTexture;

	return posTile;
}

SDL_Rect Tile::getDiamondShapeMapTilePos(int row, int col) {
	const int widthTexture = tileWidth - TilesOverlap;
	const int heightTexture = tileHeight - TilesOverlap;

	Position* pos = getDiamondShapeMapPos(row, col);
	SDL_Rect posTile;

	posTile.x = pos->getX();
	posTile.y = pos->getY();
	posTile.w = widthTexture;
	posTile.h = heightTexture;

	delete pos;

	return posTile;
}

Position* Tile::getDiamondShapeMapPos(int row, int col) {
	const int widthTexture = tileWidth - TilesOverlap;
	const int heightTexture = tileHeight - TilesOverlap;

	Position* pos = new Position();

	pos->setX((col - row) * widthTexture / 2);
	pos->setY((col + row) * heightTexture / 2);

	return pos;
}

Coordinates* Tile::getTileCoordinates(int x, int y) {
	float xF = (float) x;
	float yF = (float) y;

	float widthTexture = (float) tileWidth - (float) TilesOverlap;
	float heightTexture = (float) tileHeight - (float) TilesOverlap;

	int col = (int) (roundf(xF / widthTexture + yF / heightTexture));
	int row = (int) (roundf(yF / heightTexture - xF / widthTexture));

	Coordinates* coor = new Coordinates(row, col);
	return coor;
}

void Tile::setTileWidth(unsigned int _tileWidth) {
	tileWidth = _tileWidth;
}

void Tile::setTileHeight(unsigned int _tileHeight) {
	tileHeight = _tileHeight;
}

unsigned int Tile::getTileWidth() {
	return tileWidth;
}

unsigned int Tile::getTileHeight() {
	return tileHeight;
}

void Tile::setFScore(float _fScore) {
	fScore = _fScore;
}

float Tile::getFScore() {
	return fScore;
}

bool Tile::isNeighbor(Tile* otherTile) {
	int rowDiff = coordinates->getRow() - otherTile->getCoordinates().getRow();
	int colDiff = coordinates->getCol() - otherTile->getCoordinates().getCol();

	if (rowDiff == 0 && colDiff == 0) return false;

	if ( (rowDiff == 0 || abs(rowDiff) == 1 ) &&
		 (colDiff == 0 || abs(colDiff) == 1 ) ) {
			return true;
	}

	return false;
}

bool Tile::isEqual(Tile* otherTile) {
	Coordinates otherCoords = otherTile->getCoordinates();

	bool equalRow = (otherCoords.getRow() == coordinates->getRow());
	bool equalCol = (otherCoords.getCol() == coordinates->getCol());

	return (equalRow && equalCol);
}

int Tile::getHashValue() {
	return coordinates->getRow()*1000000 + coordinates->getCol();
}

Tile& Tile::operator=(Tile &other) {
	this->fScore = other.fScore;
	this->textureIdentifier = other.textureIdentifier;
	*(this->coordinates) = *(other.coordinates);
	*(this->position) = *(other.position);
	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Tile& tile){
	out << tile.fScore << " " << *(tile.position) << " " << *(tile.coordinates);
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Tile& tile){
	float score;
	Position* pos = new Position();
	Coordinates coord;
	in >> score;
	tile.setFScore(score);
	in >> *pos;
	tile.setPosition(pos);
	in >> coord;
	tile.setCoordinates(coord);
	return in;
}
