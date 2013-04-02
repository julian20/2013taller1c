/*
 * Tile.h
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#ifndef TILE_H_
#define TILE_H_

#include <model/map/Tile.h>
#include <model/entityProperties/Position.h>
#include <model/entityProperties/Coordinates.h>
#include <string>

class Tile {
public:
	Tile(Position* position, std::string textureId);
	virtual ~Tile();
	Position* getPosition();
	void setPosition(int x, int y, int z);
	void setPosition(Position* newPosition);
	Coordinates* getCoordinates();
	void setCoordinates(int _row, int _col);
	std::string getTextureIdentifier();
	void setTextureIdentifier(std::string textureId);
private:
	Tile();
	Position* position;
	Coordinates* coordinates;
	std::string textureIdentifier;
};

#endif /* TILE_H_ */
