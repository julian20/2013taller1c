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
	TileDefinition(std::string id, std::string imageSrc);
	virtual ~TileDefinition();
	std::string getTileId();
	void setTileId(std::string id);
	SDL_Surface* getTileImage();
	void setTileImageSrc(std::string imageSrc);
	std::string getTileImageSrc();
	SDL_Surface* getDefaultTileImage();
private:
	std::string id, imageSrc, defaultImgSrc;
	SDL_Surface* openImage;
	void setDefaultTileImage(std::string imageSrc);
	SDL_Surface* prepareImage(SDL_Surface* loadedImage);
};

#endif /* TILEDEFINITION_H_ */
