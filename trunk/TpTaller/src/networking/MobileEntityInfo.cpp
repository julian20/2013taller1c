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

	imageWidth = 0;
	imageHeight = 0;
	anchorPixel = new Vector2(0, 0);
	fps = FPS;
	delay = DELAY;

	entity = NULL;
	initCoords = NULL;
}

void MobileEntityInfo::setName(string name) {
	this->name = name;
}

string MobileEntityInfo::getName() {
	return this->name;
}

void MobileEntityInfo::setImages(map<string,string> images){
	this->images = images;
}
map<string,string> MobileEntityInfo::getImages(){
	return this->images;
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

void MobileEntityInfo::setInitCoordinates(Coordinates* coords) {
	this->initCoords = coords;
}
Coordinates* MobileEntityInfo::getInitCoordinates() {
	return this->initCoords;
}

void MobileEntityInfo::setEntity(MobileEntity* entity){
	this->entity = entity;
}

MobileEntity* MobileEntityInfo::getEntity(){
	return this->entity;
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

MobileEntityView* MobileEntityInfo::createMobileEntityView() {
	MobileEntityView* view = new MobileEntityView();
	view->setEntity(entity);
	view->setAnchorPixel(this->getAnchorPixel());
	view->setBaseSizes(entity->getBase()->getRows(),entity->getBase()->getCols());
	view->setDelay(this->getDelay());
	view->setFps(this->getFPS());
	view->setImageHeight(this->getImageHeight());
	view->setImageWidth(this->getImageWidth());

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

	int imageWidth = info.imageWidth;
	int imageHeight = info.imageHeight;
	Vector2* anchorPixel = info.anchorPixel;
	int fps = info.fps;
	float delay = info.delay;

	MobileEntity* entity = info.entity;
	Coordinates* initCoords = info.initCoords;

	out << name << " " << imageWidth << " " << imageHeight << " "
			<< anchorPixel->getX() << " " << anchorPixel->getY() << " " << fps
			<< " " << delay << " " << *initCoords << " " << *entity << " ";

	out << info.images.size() << " ";
	map<string,string> images = info.images;
	for (map<string,string>::iterator it = images.begin() ; it != images.end() ; ++it){
		out << it->first << " " << it->second << " ";
	}

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, MobileEntityInfo& info) {
	float x, y;

	string name;
	int imageWidth;
	int imageHeight;
	int fps;
	float delay;
	Coordinates* initCoords = new Coordinates();
	in >> name;
	info.setName(name);

	in >> imageWidth;
	in >> imageHeight;
	info.setImageDimentions(imageWidth, imageHeight);

	in >> x;
	in >> y;
	info.setAnchorPixel(x, y);

	in >> fps;
	in >> delay;
	info.setFPS(fps);
	info.setDelay(delay);

	in >> *initCoords;
	info.setInitCoordinates(initCoords);

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
