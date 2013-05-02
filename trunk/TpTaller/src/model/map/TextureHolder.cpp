/*
 * TextureHolder.cpp
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#include <model/map/TextureHolder.h>

#define DEFAULT_TEXTURE_ID "default"
#define DEFAULT_TEXTURE_PATH "resources/texturesTiles/grass.png"

#define DEFAULT_CHARACTER_ID	"characterDefault"
#define DEFAULT_CHARACTER_PATH	"resources/soldierSheet.png"

TextureHolder::TextureHolder() {
	TextureDefinition* defaultTile = new TextureDefinition(DEFAULT_TEXTURE_ID,
			DEFAULT_TEXTURE_PATH);
	this->listOfTextures.push_back(defaultTile);

	TextureDefinition* defaultCharacter = new TextureDefinition(
			DEFAULT_CHARACTER_ID, DEFAULT_CHARACTER_PATH);
	this->listOfTextures.push_back(defaultCharacter);
}

TextureHolder::~TextureHolder() {
	for (unsigned int i = 0; i < listOfTextures.size(); i++) {
		delete listOfTextures[i];
	}
}

bool TextureHolder::duplicateTexture(std::vector<TextureDefinition*> list,
		std::string id) {
	for (unsigned int i = 0; i < list.size(); i++) {
		TextureDefinition* auxTextureDefinition = list[i];
		if (id.compare(auxTextureDefinition->getTextureId()) == 0) {
			return true;
		}
	}

	return false;
}

void TextureHolder::addFogTexture(std::string id, SDL_Surface* texture) {
	fogTextures[id] = texture;
}

void TextureHolder::addTexture(TextureDefinition* textureDefinition) {
	if (duplicateTexture(listOfTextures, textureDefinition->getTextureId())) {
		return;
	}

	this->listOfTextures.push_back(textureDefinition);
}

std::string TextureHolder::getTextureSrc(std::string id) {
	for (unsigned int i = 0; i < this->listOfTextures.size(); i++) {
		TextureDefinition* auxTextureDefinition = this->listOfTextures[i];
		if (id.compare(auxTextureDefinition->getTextureId()) == 0) {
			return auxTextureDefinition->getTextureImageSrc();
		}
	}

	return getTextureSrc(DEFAULT_TEXTURE_ID);
}

SDL_Surface* TextureHolder::getTexture(std::string id) {
	for (unsigned int i = 0; i < this->listOfTextures.size(); i++) {
		TextureDefinition* auxTextureDefinition = this->listOfTextures[i];
		if (id.compare(auxTextureDefinition->getTextureId()) == 0) {
			SDL_Surface* surf = auxTextureDefinition->getTextureImage();
			if (surf == NULL)
				return getTexture(DEFAULT_TEXTURE_ID);
			return surf;
		}
	}

	return NULL;
}

SDL_Surface* TextureHolder::getFogTexture(std::string id) {

	if (fogTextures.find(id) == fogTextures.end()) {// si no existe en el map
		fogTextures[id] = FogCreator::getFog(getTexture(id));
	}

	return fogTextures[id];
}
