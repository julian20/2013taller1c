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
#include <model/Vector2.h>
#include <model/entityProperties/Position.h>
#include <view/timer/Timer.h>

#include <string>

using namespace std;
class EntityView {
public:
	EntityView();
	void setImagePath(std::string image_path);
	std::string getImagePath();
	virtual void setEntity(Entity* entity);
	virtual Entity* getEntity();
	virtual ~EntityView();
	Vector2* getAnchorPixel();
	void setAnchorPixel(Vector2* anchorPixel);
	void setImageWidth(int width);
	void setImageHeight(int height);
	virtual void draw(SDL_Surface* screen, Position* camera);
	void setNClips(int clips);
	void setTimer(Timer timer);
	virtual bool isMovable();
protected:

	int nClips;

	SDL_Surface* load_image(std::string urlImagen);
	Entity* entity;
	SDL_Surface* image;
	Vector2* anchorPixel;
	string imagePath;
	int imageHeight;
	int imageWidth;
	//void EstablecerClips();

	int currentClip;
	SDL_Rect clip;
	bool movable;
	Timer timer;
	int timeSinceLastAnimation;
};


#endif /* ENTITYVIEW_H */
