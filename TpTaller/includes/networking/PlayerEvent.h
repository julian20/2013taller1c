/*
 * PlayerEvent.h
 *
 *  Created on: 30/04/2013
 *      Author: tomas
 */

#ifndef PLAYEREVENT_H_
#define PLAYEREVENT_H_

#include <model/entityProperties/Coordinates.h>
#include <iostream>

using namespace std;

typedef enum {
	EVENT_MOVE,
	EVENT_RUNNING,
	EVENT_ATTACK,
	EVENT_BLOCK,
	EVENT_CANCEL_BLOCK,
	EVENT_CANCEL_ATTACK,
	EVENT_WALKING,
	EVENT_STOP,
	EVENT_CONECT,
	EVENT_DISCONECT,
	EVENT_CAST_SPELL,
	EVENT_CANCEL_CAST,
	EVENT_EARTHQUAKE_START,
	EVENT_GOLEM,
	EVENT_CAST_SHIELD,
	EVENT_FROZE_ENEMIES,
	EVENT_CHANGE_WEAPON,
	EVENT_NONE
} EventType;

class PlayerEvent {
public:
	PlayerEvent();
	PlayerEvent(EventType type);
	PlayerEvent(EventType type, Coordinates tileCoordinates);
	PlayerEvent(const PlayerEvent &toCopy);
	EventType getEventType();
	void setEventType(EventType type);
	Coordinates* getTileCoordinates();
	void setTileCoordinates(Coordinates tileCoordinates);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const PlayerEvent&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, PlayerEvent&);

	virtual ~PlayerEvent();
private:
	EventType type;
	Coordinates* tileCoordinates;
};

#endif /* PLAYEREVENT_H_ */
