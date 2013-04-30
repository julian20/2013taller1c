/*
 * EntityViewData.h
 *
 *  Created on: 13/04/2013
 *      Author: damian
 */

#ifndef ENTITYVIEWDATA_H_
#define ENTITYVIEWDATA_H_
#include <model/map/Tile.h>
#include <vector>
#include <SDL/SDL.h>
#include <string>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <model/Vector2.h>
#include  <model/Logs/Logs.h>

using namespace std;

class EntityViewData {
public:
	EntityViewData();
	virtual ~EntityViewData();
	void setEntityId(string id);
	string getEntityId();
	SDL_Surface* getEntityImage();
	void setEntityImage(string image);
	void setAnchorPixel(Vector2* anchorPixel);
	void setImageWidth(int width);
	void setImageHeight(int height);
	int getImageHeight();
	int getImageWidth();
	void setBaseWidth(int width);
	void setBaseHeight(int height);
	Vector2* getAnchorPixel();
	void setScale();
	int getNClips();
	void setNClips(int clips);
	void setFps(int nuevasFps);
	int getFps();
	int getDelay();
	void setDelay(int nuevoDelay);
	void setNumberOfRepeats(int repeat);
protected:
	//string name;
	int delay;
	int fps;
	int nOfRepeats;
	int nClips;
	int baseHeight;
	int baseWidth;
	float scaleWidth;
	float scaleHeight;
	int imageHeight;
	int imageWidth;
	Vector2* anchorPixel;
	SDL_Surface* openImage;
	string entityId;
	string entityImage;
};

#endif /* ENTITYVIEWDATA_H_ */
