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

	void setEntity(MobileEntity* entity);
	MobileEntity* getEntity();

	MobileEntityView* createMobileEntityView();
	void updatePlayer(PlayerUpdate* update);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const MobileEntityInfo&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, MobileEntityInfo&);

	virtual ~MobileEntityInfo();
private:
	int id;
	string name;

	map<string,string> images;

	Coordinates* initCoords;

	int fps;
	float delay;

	MobileEntity* entity;
};

} /* namespace std */
#endif /* MOBILEENTITYINFO_H_ */
