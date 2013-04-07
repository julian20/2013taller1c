/*
 * EntityView.h
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 *      Tuneada by: Lucas
 */

#ifndef ENTITYVIEW_H
#define ENTITYVIEW_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <model/entities/Entity.h>
#include <string>

namespace std{
	class EntityView {
	public:
		EntityView(Entity* entity, string image_path);
		virtual ~EntityView();

	protected:
		SDL_Surface* load_image(std::string urlImagen);
		Entity* entity;
		SDL_Surface* image;
	};
}

#endif /* ENTITYVIEW_H */
