/*
 * EntityView.h
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 *      Tuneada by: Lucas
 */

#ifndef ENTITYVIEW_H
#define ENTITYVIEW_H
#include <model/entities/Entity.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

class EntityView {
public:
	EntityView(Entity* entity);
	virtual ~EntityView();

protected:
	SDL_Surface* load_image(std::string urlImagen);
	Entity* entity;
};


#endif /* ENTITYVIEW_H */
