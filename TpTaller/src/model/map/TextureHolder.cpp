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
#define DEFAULT_CHARACTER_PATH	"resources/black_knight/asd.png"



TextureHolder::TextureHolder() {
	TextureDefinition* defaultTile = new TextureDefinition(DEFAULT_TEXTURE_ID,
			DEFAULT_TEXTURE_PATH);
	textures[defaultTile->getTextureId()] = defaultTile;

	TextureDefinition* defaultCharacter = new TextureDefinition(
			DEFAULT_CHARACTER_ID, DEFAULT_CHARACTER_PATH);
	textures[defaultCharacter->getTextureId()] = defaultCharacter;

	TextureDefinition* quake = new TextureDefinition(QUAKE_IMG_ID,
			QUAKE_IMG_ID);
	textures[QUAKE_IMG_ID] = quake;

	TextureDefinition* ice = new TextureDefinition(
			ICE_IMG, ICE_IMG);
			textures[ICE_IMG] = ice;

}

TextureHolder::~TextureHolder() {
	std::map<std::string, TextureDefinition*>::iterator iter;

	for (iter = textures.begin(); iter != textures.end(); ++iter) {
		string id = iter->first;
		delete textures[id];
	}

}

void TextureHolder::addFogTexture(std::string id, SDL_Surface* texture) {
	fogTextures[id] = texture;
}

void TextureHolder::addTexture(TextureDefinition* textureDefinition) {
	string id = textureDefinition->getTextureId();
	bool existInMap = !(textures.find(id) == textures.end());

	if (existInMap)
		return;

	textures[id] = textureDefinition;
}

std::string TextureHolder::getTextureSrc(std::string id) {
	bool existInMap = !(textures.find(id) == textures.end());

	if (existInMap)
		return textures[id]->getTextureImageSrc();

	return getTextureSrc(DEFAULT_TEXTURE_ID);
}

SDL_Surface* TextureHolder::getTexture(std::string id) {
	bool existInMap = !(textures.find(id) == textures.end());

	if (existInMap) {
		SDL_Surface* surf = textures[id]->getTextureImage();

		if (surf == NULL)
			return getTexture(DEFAULT_TEXTURE_ID);
		return surf;
	}

	return getTexture(DEFAULT_CHARACTER_ID);
}

bool TextureHolder::existsTexture(std::string id) {
	return !(textures.find(id) == textures.end());
}

SDL_Surface* TextureHolder::getFogTexture(std::string id) {

	if (fogTextures.find(id) == fogTextures.end()) { // si no existe en el map
		fogTextures[id] = FogCreator::getFog(getTexture(id));
	}

	return fogTextures[id];
}

map<string, string> TextureHolder::getMobileEntityImages(string entName) {
	map<string, string> playerImages;

	vector<string> modifiers = TextureDefinition::getModifiers();

	for (unsigned int i = 0; i < modifiers.size(); i++) {
		string id = entName + modifiers[i];
		if (!existsTexture(id))
			continue;
		playerImages.insert(pair<string, string>(id, getTextureSrc(id)));
	}

	return playerImages;

}
