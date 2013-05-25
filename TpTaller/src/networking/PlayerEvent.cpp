/*
 * PlayerEvent.cpp
 *
 *  Created on: 30/04/2013
 *      Author: tomas
 */

#include <networking/PlayerEvent.h>
using namespace std;

PlayerEvent::PlayerEvent(){
	this->type = EVENT_NONE;
	this->tileCoordinates = new Coordinates();
}

PlayerEvent::PlayerEvent(const PlayerEvent &toCopy){

	this->type = toCopy.type;
	int row = toCopy.tileCoordinates->getRow();
	int col= toCopy.tileCoordinates->getCol();
	this->tileCoordinates = new Coordinates(row,col);

}

PlayerEvent::PlayerEvent(EventType type){
	this->type = type;
	this->tileCoordinates = new Coordinates();
}

PlayerEvent::PlayerEvent(EventType type, Coordinates tileCoordinates){

	this->type = type;
	this->tileCoordinates = new Coordinates();
	this->tileCoordinates->setCol(tileCoordinates.getCol());
	this->tileCoordinates->setRow(tileCoordinates.getRow());

}

void PlayerEvent::setEventType(EventType type){
	this->type = type;
}

void PlayerEvent::setTileCoordinates(Coordinates tileCoordinates){
	this->tileCoordinates->setCol(tileCoordinates.getCol());
	this->tileCoordinates->setRow(tileCoordinates.getRow());
}



EventType PlayerEvent::getEventType(){
	return type;
}

Coordinates* PlayerEvent::getTileCoordinates(){
	return this->tileCoordinates;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out , const PlayerEvent& event){
	out << event.type << " " << *(event.tileCoordinates);
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, PlayerEvent& event){

	EventType type;
	int tmp;
	in >> tmp;
	type = (EventType) tmp;
	Coordinates coords;
	in >> coords;
	event.setEventType(type);
	event.setTileCoordinates(coords);

	return in;
}

PlayerEvent::~PlayerEvent() {
	delete tileCoordinates;
}

