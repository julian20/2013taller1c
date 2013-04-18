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
	TextureDefinition* defaultTile = new TextureDefinition(DEFAULT_TEXTURE_ID,DEFAULT_TEXTURE_PATH);
	this->listOfTextures.push_back(defaultTile);
}

TextureHolder::~TextureHolder() {
	for (unsigned int i = 0; i < listOfTextures.size(); i++) {
		delete listOfTextures[i];
	}
}

bool TextureHolder::duplicateTexture(std::string id) {
	for (unsigned int i = 0; i < this->listOfTextures.size(); i++) {
		TextureDefinition* auxTileDefinition = this->listOfTextures[i];
		if (id.compare(auxTileDefinition->getTextureId()) == 0) {
			return true;
		}
	}

	return false;
}

void TextureHolder::addTexture(TextureDefinition* tileDefinition) {
	if (duplicateTexture(tileDefinition->getTextureId())) {
		return;
	}

	this->listOfTextures.push_back(tileDefinition);
}

std::string TextureHolder::getTextureSrc(std::string id) {
	for (unsigned int i = 0; i < this->listOfTextures.size(); i++) {
		TextureDefinition* auxTileDefinition = this->listOfTextures[i];
		if (id.compare(auxTileDefinition->getTextureId()) == 0) {
			return auxTileDefinition->getTextureImageSrc();
		}
	}

	return getTextureSrc(DEFAULT_TEXTURE_ID);
}

SDL_Surface* TextureHolder::getTexture(std::string id) {
	for (unsigned int i = 0; i < this->listOfTextures.size(); i++) {
		TextureDefinition* auxTileDefinition = this->listOfTextures[i];
		if (id.compare(auxTileDefinition->getTextureId()) == 0) {
			SDL_Surface* surf = auxTileDefinition->getTextureImage();
			if (!surf) return getTexture(DEFAULT_TEXTURE_ID);
			return surf;
		}
	}

	return NULL;
}

