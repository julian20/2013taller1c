/*
 * PlayerUpdate.h
 *
 *  Created on: 08/05/2013
 *      Author: tomas
 */

#ifndef PLAYERUPDATE_H_
#define PLAYERUPDATE_H_

#include <sstream>
#include <list>

#include <model/map/Tile.h>
#include <model/Vector3.h>
#include <model/Chat.h>
#include <model/entityProperties/Speed.h>
#include <model/entityProperties/Coordinates.h>
#include <networking/MobileEntityUpdate.h>
#include <model/Chat.h>
using namespace std;

class PlayerUpdate : public MobileEntityUpdate {
public:
	PlayerUpdate();
	virtual ~PlayerUpdate();

	Chat* chat;
	bool isBlocking();
	void setBlocking(bool blocking);
	bool isActive();
	void setActive(bool active);
	void setChat(Chat* chat);
	void setMakingEarthquake(bool makingEarthquake);
	bool getMakingEarthquake();

	void setInvulnerable(bool invul);
	bool getIsInvulnerable();

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const PlayerUpdate&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, PlayerUpdate&);

private:
	bool blocking;
	bool active;
	bool makingEarthquake;
	bool invulnerable;
	bool golem;

};

#endif /* PLAYERUPDATE_H_ */
