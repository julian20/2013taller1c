/*
 * TileDefinition.cpp
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#include <model/map/TextureDefinition.h>
#include <SDL/SDL.h>
#include <string>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include <model/Logs/Logs.h>

#define SCALE  	1

#define DEFAULT_TEXTURE_PATH "resources/questionMark.png"

TextureDefinition::TextureDefinition() {
	this->id = "default";
	this->setDefaultImage(DEFAULT_TEXTURE_PATH);
	this->imageSrc = this->defaultImgSrc;
	this->openImage = NULL;
}

TextureDefinition::TextureDefinition(std::string id, std::string imageSrc) {
	this->setDefaultImage(DEFAULT_TEXTURE_PATH);
	this->openImage = NULL;
	this->id = id;
	this->imageSrc = imageSrc;
}

TextureDefinition::~TextureDefinition() {
	SDL_FreeSurface(openImage);
}

std::string TextureDefinition::getTextureId() {
	return this->id;
}

void TextureDefinition::setTextureId(std::string id) {
	this->id = id;
}

std::string TextureDefinition::getTextureImageSrc() {
	return this->imageSrc;
}

SDL_Surface* TextureDefinition::getTextureImage() {
	if (this->openImage == NULL) {
		SDL_Surface* loadedImageTmp = IMG_Load(this->imageSrc.c_str());
		if (loadedImageTmp == NULL) {
			Logs::logErrorMessage(
					"Unable to load tile texture: " + string(SDL_GetError()));
			loadedImageTmp = this->getDefaultTextureImage();
		}

		SDL_Surface* loadedImageRot = rotozoomSurfaceXY(loadedImageTmp, 0,
				SCALE, SCALE, 0);
		SDL_FreeSurface(loadedImageTmp);
		SDL_Surface* loadedImage = SDL_DisplayFormatAlpha(loadedImageRot);
		SDL_FreeSurface(loadedImageRot);
		this->openImage = loadedImage;
	}
	return this->openImage;
}

void TextureDefinition::setTextureImageSrc(std::string imageSrc) {
	this->imageSrc = imageSrc;
}

SDL_Surface* TextureDefinition::prepareImage(SDL_Surface* loadedImage) {
	SDL_Surface* tmp = loadedImage;
	loadedImage = rotozoomSurfaceXY(tmp, 0, SCALE, SCALE, 0);
	SDL_FreeSurface(tmp);
	tmp = loadedImage;
	loadedImage = SDL_DisplayFormatAlpha(tmp);
	SDL_FreeSurface(tmp);
	return loadedImage;
}

SDL_Surface * TextureDefinition::getDefaultTextureImage() {
	SDL_Surface* loadedImage = IMG_Load(this->defaultImgSrc.c_str());
	if (loadedImage == NULL) {
		Logs::logErrorMessage(
				"Unable to load default tile texture: "
						+ string(SDL_GetError()));
		return NULL;
	}
	loadedImage = prepareImage(loadedImage);
	return loadedImage;
}

void TextureDefinition::setDefaultImage(std::string imageSrc) {
	this->defaultImgSrc = imageSrc;
}
