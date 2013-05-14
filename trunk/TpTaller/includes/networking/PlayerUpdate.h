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

namespace std {

class PlayerUpdate {
public:
	PlayerUpdate();
	virtual ~PlayerUpdate();

	void setName(string playerName);
	string getName();
	Chat* chat;
	Vector3* getCurrentPos();
	void setCurrentPos(Vector3* currentPos);
	Vector3* getEndPos();
	void setEndPos(Vector3* endPos);
	Speed* getSpeed();
	void setSpeed(Speed* speed);
	bool isAttacking();
	void setAttacking(bool attacking);
	bool isBlocking();
	void setBlocking(bool blocking);
	Tile* getTile();
	void setTile(Tile* tile);
	Coordinates* getInitCoordinates();
	void setInitCoordinates(Coordinates* initCoords);
	void setNextTile(Tile* nextTile);
	Tile* getNextTile();

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const PlayerUpdate&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, PlayerUpdate&);

private:

	string name;

	Vector3* currentPos;
	Vector3* endPos;

	Speed* speed;

	bool attacking;
	bool blocking;

	Coordinates* initCoords;

	Tile* currentTile;
	Tile* nextTile;


};

} /* namespace std */
#endif /* PLAYERUPDATE_H_ */
