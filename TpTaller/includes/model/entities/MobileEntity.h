/*
 * MobileEntity.h
 *
 *  Created on: May 25, 2013
 *      Author: mint
 */

#ifndef MOBILEENTITY_H_
#define MOBILEENTITY_H_

#include <stdio.h>
#include <iostream>
#include <list>
#include <model/entities/Entity.h>
#include <model/Vector2.h>	// Capas hay q volarlo
#include <model/Vector3.h>
#include <networking/MobileEntityUpdate.h>
#include <networking/PlayerEvent.h>

using namespace std;
class MapData;

class MobileEntity: public Entity {
public:
	MobileEntity();
	MobileEntity(string name, Position* position, Speed* speed);
	virtual ~MobileEntity();
	void moveTo(int x, int y, int z = 0);
	void setPos(float x, float y, float z = 0);

	void update(MapData* mapData);
	void updateFromServer(MobileEntityUpdate* update);
	MobileEntityUpdate* generateMobileEntityUpdate();

	virtual string getClassName();
	void moveImmediately(Coordinates coords);
	Vector2* getMovementDirection();
	bool IsMoving();
	void setPosition(Position* position);
	Speed* getSpeed();
	void setSpeed(Speed* speed);
	void setInitSpeed(Speed* initSpeed);

	void assignPath(list<Tile *> *_path);
	bool isRunning();

	void attackTo(Entity* attackTo);
	void attack();
	void setAttack(bool attacking);
	bool isAttacking();
	void cancelAttack();

	void setHasChanged(bool change);
	bool getHasChanged();

	PlayerEvent* getPlayerEvent();

	void stop();
	Vector3* getEndPos();
	void setEndPos(float x, float y, float z);

	void collideTo(Entity* entity);
	void reverseCollide(Entity* entity);
	void attack(Entity* entity);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const MobileEntity&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, MobileEntity&);

	MobileEntity& operator=(const MobileEntity &other);

	void setSpeedMagnitude(int mag);
protected:
	void loadNextPosition();
	void emptyPath();
	Vector3* endPos;
	Speed* speed;
	Speed* initSpeed;

	list<Tile *> *path;

	bool attacking;

	bool hasChanged;
	bool addEvent;
	int team;
private:
	void checkAttackToNewPos(MapData* mapData);

	Entity* attackToEntity;
};

#endif /* MOBILEENTITY_H_ */
