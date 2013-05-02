/*
 * PlayerEvent.h
 *
 *  Created on: 30/04/2013
 *      Author: tomas
 */

#ifndef PLAYEREVENT_H_
#define PLAYEREVENT_H_

#include <model/Vector2.h>
#include <iostream>

namespace std {

typedef enum {
	EVENT_MOVE, EVENT_RUNNING, EVENT_ATTACK, EVENT_BLOCK, EVENT_CANCEL_BLOCK, EVENT_CANCEL_ATTACK, EVENT_WALKING, EVENT_NONE
}EventType;

class PlayerEvent {
public:
	PlayerEvent();
	PlayerEvent(EventType type);
	PlayerEvent(EventType type, Vector2 mousePos);
	EventType getEventType();
	void setEventType(EventType type);
	Vector2* getMousePos();
	void setMousePos(Vector2 mousePos);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const PlayerEvent&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, PlayerEvent&);

	virtual ~PlayerEvent();
private:
	EventType type;
	Vector2* mousePos;
};

} /* namespace std */
#endif /* PLAYEREVENT_H_ */
