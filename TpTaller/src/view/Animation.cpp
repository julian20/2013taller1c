/*
 * Animation.cpp
 *
 *  Created on: May 30, 2013
 *      Author: mint
 */

#include <view/Animation.h>
#include <model/Logs/Logs.h>
#define DEFAULT_SPEED 0
#define ANIMATION_DELAY 0

Animation::Animation(const char* spritePath, int nClips) {

	velocity = DEFAULT_SPEED;
	frame = currentRepeat = 0;
	numberOfRepeats = 0;
	SDL_Surface* tempImg = IMG_Load(spritePath);
	if (!tempImg) {
		Logs::logErrorMessage(
				string("Error al cargar animacion: ") + string(spritePath));
	}
	sprite = SDL_DisplayFormatAlpha(tempImg);
	SDL_FreeSurface(tempImg);

	this->nClips = nClips;

}

void Animation::repeatNTimes(int times) {
	currentRepeat = 0;
	numberOfRepeats = times;
}

void Animation::blitNextFrame(SDL_Surface* screen, SDL_Rect location) {

	if (currentRepeat < numberOfRepeats) {
		SDL_Rect image;
		image.h = sprite->h;
		image.w = sprite->w / nClips;
		image.x = frame * image.w;
		image.y = 0;

		location.x -= image.w / 2;
		location.y -= image.h / 2;

		SDL_BlitSurface(sprite, &image, screen, &location);
		if (velocity == ANIMATION_DELAY) {
			this->frame++;
			velocity = 0;
		} else
			velocity++;
		frame++;
		if (frame >= nClips){
			frame = 0;
			currentRepeat++;
		}

	}

}

Animation::~Animation() {
	SDL_FreeSurface(sprite);
}

