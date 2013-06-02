/*
 * MobileEntityInfo.cpp
 *
 *  Created on: 02/06/2013
 *      Author: tomas
 */

#include <networking/MobInfo.h>
#include <model/map/TextureHolder.h>
#include <model/map/TextureDefinition.h>

#define FPS  30
#define DELAY  1

namespace std {

MobInfo::MobInfo(){

	name = string("DEFAULT");

	entity = NULL;
	initCoords = NULL;
}

void MobInfo::setName(string name) {
	this->name = name;
}

string MobInfo::getName() {
	return this->name;
}



void MobInfo::setInitCoordinates(Coordinates* coords) {
	this->initCoords = coords;
}
Coordinates* MobInfo::getInitCoordinates() {
	return this->initCoords;
}

void MobInfo::setEntity(MobileEntity* entity){
	this->entity = entity;
}

MobileEntity* MobInfo::getEntity(){
	return this->entity;
}


MobileEntityView* MobInfo::createMobileEntityView() {
	MobileEntityView* view = new MobileEntityView();
	view->setEntity(entity);

	return view;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const MobInfo& info) {
	string name = info.name;


	MobileEntity* entity = info.entity;
	Coordinates* initCoords = info.initCoords;

	out << info.id << " " << name << " " <<  *initCoords << " " << *entity << " " ;


	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, MobInfo& info) {
	float x, y;

	string name;
	int imageWidth;
	int imageHeight;
	int fps;
	float delay;
	Coordinates* initCoords = new Coordinates();
	in >> name;
	info.setName(name);



	in >> *initCoords;
	info.setInitCoordinates(initCoords);

	in >> *info.entity;

	return in;
}

MobInfo::~MobInfo() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
