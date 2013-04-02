/* 
 * File:   PersonajeVista.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:25
 */

#include <view/entities/PersonajeVista.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <cmath>
#include <string>

EntityView::EntityView(Entity* entity){
	this->entity=entity;
}

SDL_Surface* EntityView::load_image(std::string urlImagen) {
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized surface that will be used
	SDL_Surface* optimizedImage = NULL;

	loadedImage = IMG_Load(urlImagen.c_str());

	if (loadedImage != NULL) {
		//loadedImage = rotozoomSurfaceXY(loadedImage, 0, SCALE, SCALE, 0);

		//Create an optimized surface
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);

		SDL_FreeSurface(loadedImage);

		if (optimizedImage != NULL) {
			//Color key surface
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY,
					SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		}
	}

	return optimizedImage;
}

EntityView::~EntityView(){
}

