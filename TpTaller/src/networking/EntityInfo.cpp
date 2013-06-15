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

void EntityInfo::setInitCoordinates(Coordinates* coords) {
	this->initCoords = coords;
}
Coordinates* EntityInfo::getInitCoordinates() {
	return this->initCoords;
}

void EntityInfo::setClassName(string name) {
	this->className = name;
}

string EntityInfo::getClassName() {
	return this->className;
}


void EntityInfo::setEntity(Entity* entity){
	this->entity = entity;
}

Entity* EntityInfo::getEntity(){
	return this->entity;
}


EntityView* EntityInfo::createEntityView(ItemViewHolder* vh) {
	EntityView* view = vh->getEntityView(className);

	view->setEntity(entity);
	view->setName(name);
	return view;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const EntityInfo& info) {
	string name = info.name;


	Entity* entity = info.entity;
	Coordinates* initCoords = info.initCoords;

	out << info.id << " " << name << " " <<  *initCoords << " " << *entity << " " << info.className;

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, EntityInfo& info) {
	Coordinates* initCoords = new Coordinates();
	in >> info.id;
	in >> info.name;

	in >> *initCoords;
	info.setInitCoordinates(initCoords);

	info.entity = new Entity();

	in >> *info.entity;
	in >> info.className;

	return in;
}

EntityInfo::~EntityInfo() {
	// TODO Auto-generated destructor stub
}


