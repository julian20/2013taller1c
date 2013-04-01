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
#include <string>

class Tile {
public:
	Tile(Position* position, std::string textureId);
	virtual ~Tile();
	Position* getPosition();
	void setPosition(Position* position);
	std::string getTextureIdentifier();
	void setTextureIdentifier(std::string textureId);
private:
	Tile();
	Position* position;
	std::string textureIdentifier;
};

#endif /* TILE_H_ */
