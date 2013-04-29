/*
 * FogCreator.h
 *
 *  Created on: 28/04/2013
 *      Author: champ
 */

#ifndef FOGCREATOR_H_
#define FOGCREATOR_H_

#include <model/Logs/Logs.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

class FogCreator {
public:
	FogCreator();
	virtual ~FogCreator();

	static SDL_Surface* getFog(SDL_Surface* image);
private:
	static void setPixelInvisible(SDL_Surface * surface, int x, int y);
	static SDL_Surface* createFogSurface(int width, int heigth);
};

#endif /* FOGCREATOR_H_ */
