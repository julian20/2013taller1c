/*
 * TileDefinition.cpp
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#include <model/map/TileDefinition.h>
#include <SDL/SDL.h>
#include <string>
#include <SDL/SDL_image.h>

TileDefinition::TileDefinition() {
	this->id = "default";
	this->setDefaultTileImage("defaultTexturePath");
	this->imageSrc = this->defaultImgSrc;
}

TileDefinition::~TileDefinition() {
}

std::string TileDefinition::getTileId() {
	return this->id;
}

void TileDefinition::setTileId(std::string id) {
	this->id = id;
}

SDL_Surface* TileDefinition::getTileImage() {
	SDL_Surface* loadedImage = IMG_Load(this->imageSrc.c_str());
	if (loadedImage == NULL) {
		printf("Unable to load tile texture. %s\n", SDL_GetError());
		loadedImage = this->getDefaultTileImage();
	}
	return loadedImage;
}

void TileDefinition::setTileImageSrc(std::string imageSrc) {
	this->imageSrc = imageSrc;
}

SDL_Surface* TileDefinition::getDefaultTileImage() {
	SDL_Surface* loadedImage = IMG_Load(this->defaultImgSrc.c_str());
	if (loadedImage == NULL) {
		printf("Unable to load tile texture. %s\n", SDL_GetError());
		exit(1);
	}
	return loadedImage;
}

void TileDefinition::setDefaultTileImage(std::string imageSrc) {
	this->defaultImgSrc = imageSrc;
}
