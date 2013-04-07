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
#include <SDL/SDL.h>
#include <string>

class Tile {
public:
	Tile();
	Tile(Position* position, std::string textureId);
	Tile(Coordinates* _coordinates);
	virtual ~Tile();
	Position* getPosition();
	void setPosition(int x, int y, int z);
	void setPosition(Position* newPosition);
	Coordinates* getCoordinates();
	void setCoordinates(int _row, int _col);
	std::string getTextureIdentifier();
	void setTextureIdentifier(std::string textureId);
	static SDL_Rect computePosition(int row, int col);
	static SDL_Rect computePosition(int row, int col, bool toTileCenter);
	static Coordinates* getTileCoordinates(int x, int y);
private:


	static SDL_Rect getSquaredMapTilePos(int row, int col);
	static SDL_Rect getDiamondShapeMapTilePos(int row, int col);

	Position* position;
	Coordinates* coordinates;
	std::string textureIdentifier;
};

#endif /* TILE_H_ */
