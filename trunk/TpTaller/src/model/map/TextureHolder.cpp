/*
 * TextureHolder.cpp
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#include <model/map/TextureHolder.h>

#define DEFAULT_TEXTURE_ID "default"
#define DEFAULT_TEXTURE_PATH "resources/texturesTiles/grass.png"

TextureHolder::TextureHolder() {
	TileDefinition* defaultTile = new TileDefinition(DEFAULT_TEXTURE_ID,DEFAULT_TEXTURE_PATH);
	this->listOfTextures.push_back(defaultTile);
}

TextureHolder::~TextureHolder() {
	for (unsigned int i = 0; i < listOfTextures.size(); i++) {
		delete listOfTextures[i];
	}
}

bool TextureHolder::duplicateTexture(std::string id) {
	for (unsigned int i = 0; i < this->listOfTextures.size(); i++) {
		TileDefinition* auxTileDefinition = this->listOfTextures[i];
		if (id.compare(auxTileDefinition->getTileId()) == 0) {
			return true;
		}
	}

	return false;
}

void TextureHolder::addTexture(TileDefinition* tileDefinition) {
	if (duplicateTexture(tileDefinition->getTileId())) {
		return;
	}

	this->listOfTextures.push_back(tileDefinition);
}

std::string TextureHolder::getTextureSrc(std::string id) {
	for (unsigned int i = 0; i < this->listOfTextures.size(); i++) {
		TileDefinition* auxTileDefinition = this->listOfTextures[i];
		if (id.compare(auxTileDefinition->getTileId()) == 0) {
			return auxTileDefinition->getTileImageSrc();
		}
	}

	return getTextureSrc(DEFAULT_TEXTURE_ID);
}

SDL_Surface* TextureHolder::getTexture(std::string id) {
	for (unsigned int i = 0; i < this->listOfTextures.size(); i++) {
		TileDefinition* auxTileDefinition = this->listOfTextures[i];
		if (id.compare(auxTileDefinition->getTileId()) == 0) {
			SDL_Surface* surf = auxTileDefinition->getTileImage();
			if (!surf) return getTexture(DEFAULT_TEXTURE_ID);
			return surf;
		}
	}

	return NULL;
}

