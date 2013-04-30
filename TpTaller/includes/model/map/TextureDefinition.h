/*
 * TileDefinition.h
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#ifndef TEXTUREDEFINITION_H_
#define TEXTUREDEFINITION_H_

#include <SDL/SDL.h>
#include <string>

#define ATTACK_MODIFIER "Attack"
#define WALKING_MODIFIER "Walking"
#define IDLE_MODIFIER "Idle"
#define RUNNING_MODIFIER "Running"
#define IDLE_BLOCKING_MODIFIER "IdleBlocking"

class TextureDefinition {
public:
	TextureDefinition();
	TextureDefinition(std::string id, std::string imageSrc);
	virtual ~TextureDefinition();
	std::string getTextureId();
	void setTextureId(std::string id);
	SDL_Surface* getTextureImage();
	void setTextureImageSrc(std::string imageSrc);
	std::string getTextureImageSrc();
	SDL_Surface* getDefaultTextureImage();
private:
	std::string id, imageSrc, defaultImgSrc;
	SDL_Surface* openImage;
	void setDefaultImage(std::string imageSrc);
	SDL_Surface* prepareImage(SDL_Surface* loadedImage);
};

#endif /* TEXTUREDEFINITION_H_ */
