/*
 * Animation.h
 *
 *  Created on: May 30, 2013
 *      Author: mint
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
class Animation {
public:
//Initializes the variables
	Animation(const char* spritePath, int nClips);
	void blitNextFrame(SDL_Surface* screen, SDL_Rect location);
	void repeatNTimes(int times);
	~Animation();

private:
	int nClips;
	int velocity;
	int frame;
	int numberOfRepeats;
	int currentRepeat;
	SDL_Surface* sprite;
};

#endif /* ANIMATION_H_ */
