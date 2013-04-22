/*
 * TextureHolder.h
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#ifndef TEXTUREHOLDER_H_
#define TEXTUREHOLDER_H_

#include <model/map/TextureDefinition.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

class TextureHolder {
public:
	TextureHolder();
	virtual ~TextureHolder();
	void addTexture(TextureDefinition* textureDefinition);
	void addFogTexture(std::string id, SDL_Surface* texture);
	SDL_Surface* getTexture(std::string id);
	SDL_Surface* getFogTexture(std::string id);
	std::string getTextureSrc(std::string id);
private:
	std::vector<TextureDefinition*> listOfTextures;
	std::map<std::string, SDL_Surface*> fogTextures;
	bool duplicateTexture(std::vector<TextureDefinition*> list, std::string id);
};

#endif /* TEXTUREHOLDER_H_ */
