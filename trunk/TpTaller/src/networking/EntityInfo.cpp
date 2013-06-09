/*
 * EntityInfo.cpp
 *
 *  Created on: 08/06/2013
 *      Author: tomas
 */
#include <networking/EntityInfo.h>
#include <model/map/TextureHolder.h>
#include <model/map/TextureDefinition.h>

#define FPS  30
#define DELAY  1

using namespace std;

EntityInfo::EntityInfo(){

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

void EntityInfo::setName(string name) {
	this->name = name;
}

string EntityInfo::getName() {
	return this->name;
}

void EntityInfo::setId(int id){
	this->id = id;
}

int EntityInfo::getId(){
	return id;
}

void EntityInfo::setImages(map<string,string> images){
	this->images = images;
}
map<string,string> EntityInfo::getImages(){
	return this->images;
}


void EntityInfo::setInitCoordinates(Coordinates* coords) {
	this->initCoords = coords;
}
Coordinates* EntityInfo::getInitCoordinates() {
	return this->initCoords;
}

void EntityInfo::setDelay(float delay) {
	this->delay = delay;
}
float EntityInfo::getDelay() {
	return this->delay;

}

void EntityInfo::setFPS(int fps) {
	this->fps = fps;
}
int EntityInfo::getFPS() {
	return this->fps;
}

void EntityInfo::setNClips(int nClips) {
	this->nClips = nClips;
}
int EntityInfo::getNClips() {
	return this->nClips;
}


void EntityInfo::setImageDimentions(int width, int height) {
	this->imageHeight = height;
	this->imageWidth = width;
}
int EntityInfo::getImageWidth() {
	return this->imageWidth;
}
int EntityInfo::getImageHeight() {
	return this->imageHeight;
}

void EntityInfo::setAnchorPixel(float x, float y) {
	this->anchorPixel->setValues(x, y);
}
void EntityInfo::setAnchorPixel(Vector2* anchorPixel) {
	*(this->anchorPixel) = *anchorPixel;
}
Vector2* EntityInfo::getAnchorPixel() {
	return this->anchorPixel;
}



void EntityInfo::setEntity(Entity* entity){
	this->entity = entity;
}

Entity* EntityInfo::getEntity(){
	return this->entity;
}


EntityView* EntityInfo::createEntityView() {
	EntityView* view = new EntityView();
	view->setName(name);
	view->setDelay(delay);
	view->setFps(fps);
	view->setImageHeight(imageHeight);
	view->setImageWidth(imageWidth);
	view->setAnchorPixel(anchorPixel);
	view->setEntity(entity);
	view->setNClips(nClips);

	TextureHolder* th = new TextureHolder();

	for (map<string,string>::iterator it = images.begin() ; it != images.end() ; ++it){
		th->addTexture(new TextureDefinition(it->first,it->second));
	}

	view->setTextureHolder(th);

	return view;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const EntityInfo& info) {
	string name = info.name;


	Entity* entity = info.entity;
	Coordinates* initCoords = info.initCoords;

	out << info.id << " " << name << " " << *info.anchorPixel << " " << info.imageHeight << " " << info.imageWidth <<  " " <<
			info.fps << " " << info.nClips << " " << info.delay << " " <<  *initCoords << " " << *entity << " " ;

	out << info.images.size() << " ";
	map<string,string> images = info.images;
	for (map<string,string>::iterator it = images.begin() ; it != images.end() ; ++it){
		out << it->first << " " << it->second << " ";
	}

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, EntityInfo& info) {
	Coordinates* initCoords = new Coordinates();
	in >> info.id;
	in >> info.name;

	info.anchorPixel = new Vector2();
	in >> *info.anchorPixel;
	in >> info.imageHeight;
	in >> info.imageWidth;

	in >> info.fps;
	in >> info.nClips;
	in >> info.delay;

	in >> *initCoords;
	info.setInitCoordinates(initCoords);

	info.entity = new Entity();

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

EntityInfo::~EntityInfo() {
	// TODO Auto-generated destructor stub
}


