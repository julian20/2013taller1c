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
#include <model/map/Tile.h>


using namespace std;
class EntityView {
public:
	EntityView();
	void setImagePath(std::string image_path);
	std::string getImagePath();
	virtual void setEntity(Entity* entity);
	virtual Entity* getEntity();
	virtual ~EntityView();
	SDL_Surface* load_image(std::string urlImagen);
	Vector2* getAnchorPixel();
	void setAnchorPixel(Vector2* anchorPixel);
	void setImageWidth(int width);
	void setImageHeight(int height);
	void setTileWidth(int width);
	void setTileHeight(int height);
	virtual void draw(SDL_Surface* screen, Position* camera);
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

	void setBaseWidth(int width);
	void setBaseHeight(int height);
	std::string getName();
	void setName(std::string name);

protected:

	int nClips;
	int timeSinceLastAnimation;
	bool movable;
	Entity* entity;
	SDL_Surface* image;
	Vector2* anchorPixel;
	string imagePath;
	std::string name;
	int imageHeight;
	int imageWidth;
	int baseWidth;
	int baseHeight;
	float scaleWidth;
	float scaleHeight;
	int tileHeight;
	int tileWidth;
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
