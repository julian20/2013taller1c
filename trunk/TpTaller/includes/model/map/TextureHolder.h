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

class TextureHolder {
public:
	TextureHolder();
	virtual ~TextureHolder();
	void addTexture(TextureDefinition* tileDefinition);
	SDL_Surface* getTexture(std::string id);
	std::string getTextureSrc(std::string id);
private:
	std::vector<TextureDefinition*> listOfTextures;
	bool duplicateTexture(std::string id);
};

#endif /* TEXTUREHOLDER_H_ */
