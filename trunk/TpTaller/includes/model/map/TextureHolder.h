/*
 * TextureHolder.h
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#ifndef TEXTUREHOLDER_H_
#define TEXTUREHOLDER_H_

#include <view/FogCreator.h>
#include <model/map/TextureDefinition.h>
#include <vector>
#include <string>
#include <map>

#define QUAKE_IMG_ID "resources/spells/quake.png"
#define ICE_IMG "resources/spells/ice.png"

using namespace std;

class TextureHolder {
public:
	TextureHolder();
	virtual ~TextureHolder();
	void addTexture(TextureDefinition* textureDefinition);
	void addFogTexture(std::string id, SDL_Surface* texture);
	bool existsTexture(std::string id);
	SDL_Surface* getTexture(std::string id);
	SDL_Surface* getFogTexture(std::string id);

	map<string,string> getMobileEntityImages(string playerName);

	std::string getTextureSrc(std::string id);
private:
	std::map<std::string, TextureDefinition*> textures;
	std::map<std::string, SDL_Surface*> fogTextures;
	bool duplicateTexture(std::vector<TextureDefinition*> list, std::string id);
};

#endif /* TEXTUREHOLDER_H_ */
