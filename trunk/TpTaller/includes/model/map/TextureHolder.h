/*
 * TextureHolder.h
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#ifndef TEXTUREHOLDER_H_
#define TEXTUREHOLDER_H_

#include <model/map/TileDefinition.h>

#include <vector>
#include <string>

class TextureHolder {
public:
	TextureHolder();
	virtual ~TextureHolder();
	void addTexture(TileDefinition* tileDefinition);
	SDL_Surface* getTexture(std::string id);
private:
	std::vector<TileDefinition*> listOfTextures;
	bool duplicateTexture(std::string id);
};

#endif /* TEXTUREHOLDER_H_ */
