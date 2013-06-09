/*
 * EntityInfo.h
 *
 *  Created on: 08/06/2013
 *      Author: tomas
 */

#ifndef ENTITYINFO_H_
#define ENTITYINFO_H_

#include <model/Vector2.h>
#include <model/Vector3.h>
#include <model/entities/Entity.h>
#include <view/entities/EntityView.h>

#include <string>

using namespace std;

class EntityInfo {
public:
	EntityInfo();

	void setId(int id);
	int getId();

	void setName(string name);
	string getName();

	void setImages(map<string,string> images);
	map<string,string> getImages();

	void setInitCoordinates(Coordinates* coords);
	Coordinates* getInitCoordinates();

	void setDelay(float delay) ;
	float getDelay();

	void setFPS(int fps);
	int getFPS();

	void setNClips(int nClips);
	int getNClips();


	void setImageDimentions(int width, int height);
	int getImageWidth();
	int getImageHeight();

	void setAnchorPixel(float x, float y);
	void setAnchorPixel(Vector2* anchorPixel);
	Vector2* getAnchorPixel();



	void setEntity(Entity* entity);
	Entity* getEntity();

	EntityView* createEntityView();

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const EntityInfo&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, EntityInfo&);

	virtual ~EntityInfo();
private:
	int id;
	string name;

	map<string,string> images;

	Coordinates* initCoords;

	int fps;
	float delay;
	int nClips;

	int imageWidth, imageHeight;
	Vector2* anchorPixel;

	Entity* entity;
};

#endif
