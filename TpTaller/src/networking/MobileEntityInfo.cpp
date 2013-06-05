/*
 * MobileEntityInfo.cpp
 *
 *  Created on: 02/06/2013
 *      Author: tomas
 */

#include <networking/MobileEntityInfo.h>
#include <model/map/TextureHolder.h>
#include <model/map/TextureDefinition.h>

#define FPS  30
#define DELAY  1

namespace std {

MobileEntityInfo::MobileEntityInfo(){

	name = string("DEFAULT");
	id = 0;
	entity = NULL;
	initCoords = NULL;
}

void MobileEntityInfo::setName(string name) {
	this->name = name;
}

string MobileEntityInfo::getName() {
	return this->name;
}

void MobileEntityInfo::setId(int id){
	this->id = id;
}

int MobileEntityInfo::getId(){
	return id;
}

void MobileEntityInfo::setImages(map<string,string> images){
	this->images = images;
}
map<string,string> MobileEntityInfo::getImages(){
	return this->images;
}


void MobileEntityInfo::setInitCoordinates(Coordinates* coords) {
	this->initCoords = coords;
}
Coordinates* MobileEntityInfo::getInitCoordinates() {
	return this->initCoords;
}

void MobileEntityInfo::setDelay(float delay) {
	this->delay = delay;
}
float MobileEntityInfo::getDelay() {
	return this->delay;

}

void MobileEntityInfo::setFPS(int fps) {
	this->fps = fps;
}
int MobileEntityInfo::getFPS() {
	return this->fps;
}

void MobileEntityInfo::setEntity(MobileEntity* entity){
	this->entity = entity;
}

MobileEntity* MobileEntityInfo::getEntity(){
	return this->entity;
}


MobileEntityView* MobileEntityInfo::createMobileEntityView() {
	MobileEntityView* view = new MobileEntityView();
	view->setEntity(entity);

	return view;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const MobileEntityInfo& info) {
	string name = info.name;


	MobileEntity* entity = info.entity;
	Coordinates* initCoords = info.initCoords;

	out << info.id << " " << name << " " <<  *initCoords << " " << *entity << " " ;


	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, MobileEntityInfo& info) {
	string name;
	Coordinates* initCoords = new Coordinates();
	in >> name;
	info.setName(name);



	in >> *initCoords;
	info.setInitCoordinates(initCoords);

	in >> *info.entity;

	return in;
}

MobileEntityInfo::~MobileEntityInfo() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
