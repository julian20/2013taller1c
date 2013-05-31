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
#include <vector>


// LEER ANTES DE AGREGAR UN MODIFIER
// AL AGREGAR UNO NUEVO, HAY QUE AGREGARLO TAMBIEN EN EL VECTOR DEL METODO getModifiers.
// ATENCION!!!
#define ATTACK_MODIFIER "Attack"
#define WALKING_MODIFIER "Walking"
#define IDLE_MODIFIER "Idle"
#define RUNNING_MODIFIER "Running"
#define IDLE_BLOCKING_MODIFIER "IdleBlocking"
#define BOW_ATTACK_MODIFIER "AttackBow"
#define BOW_WALKING_MODIFIER "WalkingBow"
#define BOW_IDLE_MODIFIER "IdleBow"

using namespace std;


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

	static vector<string> getModifiers();
private:
	std::string id, imageSrc, defaultImgSrc;
	SDL_Surface* openImage;
	void setDefaultImage(std::string imageSrc);
	SDL_Surface* prepareImage(SDL_Surface* loadedImage);
};

#endif /* TEXTUREDEFINITION_H_ */
