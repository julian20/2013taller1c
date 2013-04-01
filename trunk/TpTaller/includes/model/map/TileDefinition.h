/*
 * TileDefinition.h
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#ifndef TILEDEFINITION_H_
#define TILEDEFINITION_H_

#include <SDL/SDL.h>
#include <string>

class TileDefinition {
public:
	TileDefinition();
	virtual ~TileDefinition();
	std::string getTileId();
	void setTileId(std::string id);
	SDL_Surface* getTileImage();
	void setTileImageSrc(std::string imageSrc);
	SDL_Surface* getDefaultTileImage();
private:
	std::string id, imageSrc, defaultImgSrc;
	void setDefaultTileImage(std::string imageSrc);
};

#endif /* TILEDEFINITION_H_ */
