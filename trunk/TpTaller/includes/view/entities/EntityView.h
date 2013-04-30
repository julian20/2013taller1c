/*
 * EntityView.h
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 *      Tuneada by: Lucas
 */

#ifndef ENTITYVIEW_H
#define ENTITYVIEW_H

#include <model/entityProperties/Position.h>
#include <model/entityProperties/Base.h>
#include <model/map/TextureDefinition.h>
#include <model/map/TextureHolder.h>
#include <model/entities/Entity.h>
#include <view/timer/Timer.h>
#include <view/FogCreator.h>
#include <model/map/Tile.h>
#include <model/Vector2.h>
#include <string>
#include <map>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


using namespace std;
class EntityView {
public:
	EntityView();
	void setTextureHolder(TextureHolder* _textureHolder);
	TextureHolder* getTextureHolder();
	virtual void setEntity(Entity* entity);
	virtual Entity* getEntity();
	virtual ~EntityView();
	Vector2* getAnchorPixel();
	void setAnchorPixel(Vector2* anchorPixel);
	void setImageWidth(int width);
	void setImageHeight(int height);
	virtual void draw(SDL_Surface* screen, Position* camera, bool drawFog);
	void setNClips(int clips);

	int getFps();
	int getDelay();
	void setDelay(int nuevoDelay);
	void setFps(int nuevasFps);
	void setNumberOfRepeats(int repeats);
	int getNumberOfRepeats();
	void setScale();
	bool isMovable();
	int getNClips();
	int getImageHeight();
	int getImageWidth();

	void setBaseSizes(int width, int height);
	int getBaseWidth();
	int getBaseHeight();
	std::string getName();
	virtual void setName(std::string name);

protected:
	void loadImage();
	void updateBaseSize();

	int nClips;
	int timeSinceLastAnimation;
	bool movable;
	Entity* entity;
	SDL_Surface* image;
	SDL_Surface* fogImage;
	Vector2* anchorPixel;
	TextureHolder* textureHolder;
	std::string name;
	int imageHeight;
	int imageWidth;
	Base* base;
	float scaleWidth;
	float scaleHeight;
	//void EstablecerClips();

	int currentClip;
	SDL_Rect clip;
	Timer timer;
	Timer animationRateTimer;

	int delay;
	int fps;

	int numberOfRepeats;
	int currentRepeat;
};


#endif /* ENTITYVIEW_H */
