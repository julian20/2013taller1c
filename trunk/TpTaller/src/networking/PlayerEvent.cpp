/*
 * PlayerEvent.cpp
 *
 *  Created on: 30/04/2013
 *      Author: tomas
 */

#include <networking/PlayerEvent.h>

namespace std {

PlayerEvent::PlayerEvent(){
	this->type = EVENT_NONE;
	this->mousePos = new Vector2(0,0);
}

PlayerEvent::PlayerEvent(EventType type){
	this->type = type;
	this->mousePos = new Vector2(0,0);
}

PlayerEvent::PlayerEvent(EventType type, Vector2 mousePos){

	this->type = type;
	this->mousePos = new Vector2(0,0);
	this->mousePos->setValues(mousePos.getX(), mousePos.getY());

}

void PlayerEvent::setEventType(EventType type){
	this->type = type;
}
void PlayerEvent::setMousePos(Vector2 mousePos){
	this->mousePos->setValues(mousePos.getX(), mousePos.getY());
}



EventType PlayerEvent::getEventType(){
	return type;
}
Vector2* PlayerEvent::getMousePos(){
	return mousePos;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out , const PlayerEvent& event){
	out << event.type << " " << event.mousePos;
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, PlayerEvent& event){

	EventType type;
	int tmp;
	in >> tmp;
	type = (EventType) tmp;
	Vector2 vector;
	in >> vector;
	event.setEventType(type);
	event.setMousePos(vector);

	return in;
}

PlayerEvent::~PlayerEvent() {
	delete mousePos;
}

} /* namespace std */
