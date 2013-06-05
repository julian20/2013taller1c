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
	imageHeight = 0;
	imageWidth = 0;
	fps = 0;
	delay = 0;
	anchorPixel = new Vector2();
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


void MobileEntityInfo::setImageDimentions(int width, int height) {
	this->imageHeight = height;
	this->imageWidth = width;
}
int MobileEntityInfo::getImageWidth() {
	return this->imageWidth;
}
int MobileEntityInfo::getImageHeight() {
	return this->imageHeight;
}

void MobileEntityInfo::setAnchorPixel(float x, float y) {
	this->anchorPixel->setValues(x, y);
}
void MobileEntityInfo::setAnchorPixel(Vector2* anchorPixel) {
	*(this->anchorPixel) = *anchorPixel;
}
Vector2* MobileEntityInfo::getAnchorPixel() {
	return this->anchorPixel;
}



void MobileEntityInfo::setEntity(MobileEntity* entity){
	this->entity = entity;
}

MobileEntity* MobileEntityInfo::getEntity(){
	return this->entity;
}


MobileEntityView* MobileEntityInfo::createMobileEntityView() {
	MobileEntityView* view = new MobileEntityView();
	view->setName(name);
	view->setDelay(delay);
	view->setFps(fps);
	view->setImageHeight(imageHeight);
	view->setImageWidth(imageWidth);
	view->setAnchorPixel(anchorPixel);
	view->setEntity(entity);

	TextureHolder* th = new TextureHolder();

	for (map<string,string>::iterator it = images.begin() ; it != images.end() ; ++it){
		th->addTexture(new TextureDefinition(it->first,it->second));
	}

	view->setTextureHolder(th);

	return view;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const MobileEntityInfo& info) {
	string name = info.name;


	MobileEntity* entity = info.entity;
	Coordinates* initCoords = info.initCoords;

	out << info.id << " " << name << " " << *info.anchorPixel << " " << info.imageHeight << " " << info.imageWidth <<  " " <<
			info.fps << " " << info.delay << " " <<  *initCoords << " " << *entity << " " ;

	out << info.images.size() << " ";
	map<string,string> images = info.images;
	for (map<string,string>::iterator it = images.begin() ; it != images.end() ; ++it){
		out << it->first << " " << it->second << " ";
	}

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, MobileEntityInfo& info) {
	Coordinates* initCoords = new Coordinates();
	in >> info.id;
	in >> info.name;

	info.anchorPixel = new Vector2();
	in >> *info.anchorPixel;
	in >> info.imageHeight;
	in >> info.imageWidth;

	in >> info.fps;
	in >> info.delay;

	in >> *initCoords;
	info.setInitCoordinates(initCoords);

	info.entity = new MobileEntity();

	in >> *info.entity;

	int size;
	in >> size;
	for (int i = 0 ; i < size ; i++){
		string id;
		string imagePath;
		in >> id;
		in >> imagePath;
		info.images.insert(pair<string,string>(id,imagePath));
	}

	return in;
}

MobileEntityInfo::~MobileEntityInfo() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
