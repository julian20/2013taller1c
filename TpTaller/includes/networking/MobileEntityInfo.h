/*
 * MobileEntityInfo.h
 *
 *  Created on: 02/06/2013
 *      Author: tomas
 */

#ifndef MOBILEENTITYINFO_H_
#define MOBILEENTITYINFO_H_

#include <model/Vector2.h>
#include <model/Vector3.h>
#include <model/entities/MobileEntity.h>
#include <view/entities/MobileEntityView.h>

#include <string>

namespace std {

class MobileEntityInfo {
public:
	MobileEntityInfo();

	void setName(string name);
	string getName();

	void setImages(map<string,string> images);
	map<string,string> getImages();

	void setImageDimentions(int width, int height);
	int getImageWidth();
	int getImageHeight();

	void setAnchorPixel(float x, float y);
	void setAnchorPixel(Vector2* anchorPixel);
	Vector2* getAnchorPixel();

	void setInitCoordinates(Coordinates* coords);
	Coordinates* getInitCoordinates();

	void setEntity(MobileEntity* entity);
	MobileEntity* getEntity();

	void setDelay(float delay);
	float getDelay();

	void setFPS(int fps);
	int getFPS();

	MobileEntityView* createMobileEntityView();
	void updatePlayer(PlayerUpdate* update);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const MobileEntityInfo&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, MobileEntityInfo&);

	virtual ~MobileEntityInfo();
private:
	string name;

	map<string,string> images;

	int imageWidth, imageHeight;
	Vector2* anchorPixel;
	int fps;
	float delay;
	Coordinates* initCoords;

	MobileEntity* entity;
};

} /* namespace std */
#endif /* MOBILEENTITYINFO_H_ */
