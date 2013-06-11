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
#include <view/timer/Timer.h>
#include <queue>

#define ATTACK_TIMEOUT			1000
#define MAGIC_DAMAGE_TIMEOUT	1000
#define FROZEN_TIMEOUT			5000
#define MAXSPEED				15

using namespace std;
class MapData;
class Item;
class LifeHeart;

class MobileEntity: public Entity {
public:
	MobileEntity();
	MobileEntity(string name, Position* position, Speed* speed);
	virtual ~MobileEntity();
	void moveTo(int x, int y, int z = 0);
	void setPos(float x, float y, float z = 0);
	void setPos(Position* pos);

	virtual void extraUpdate(MapData* mapData);
	void update(MapData* mapData);
	void updateFromServer(MobileEntityUpdate* update);
	MobileEntityUpdate* generateMobileEntityUpdate(int id);

	void setViewRange(int _viewRange);
	int getViewRange();

	virtual string getClassName();
	void moveImmediately(Coordinates coords);
	Vector2* getMovementDirection();
	bool IsMoving();
	void setPosition(Position* position);
	Speed* getSpeed();
	void setSpeed(Speed* speed);

	void setInitSpeed(Speed* initSpeed);
	void setInitSpeedMagnitude(int initSpeed);
	Speed* getInitSpeed();

	void assignPath(list<Tile *> *_path);
	bool isRunning();

	void froze();
	bool isFrozen();
	void attackTo(Entity* attackTo);
	Entity* getAttackToEntity();
	void attack();
	void setAttack(bool attacking);
	bool isAttacking();
	void cancelAttack();

	void setHasChanged(bool change);
	bool getHasChanged();

	void stop();
	Vector3* getEndPos();
	void setEndPos(float x, float y, float z);

	virtual void collideTo(Entity& entity);
	virtual void reverseCollide(Entity& entity);

	virtual void attack(Entity& entity);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const MobileEntity&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, MobileEntity&);

	MobileEntity& operator=(const MobileEntity &other);

	void setSpeedMagnitude(int mag);

	virtual Tile* getTile();
	list<PlayerEvent*> getPlayerEvents();
	void addEvent(PlayerEvent* event);
	float getLastAttackingDirecton();

	void localUpdate(MapData* mapData);
protected:
	void loadNextPosition(MapData* mapData, bool checkNextPosition = true);
	void emptyPath();
	Vector3* endPos;
	Speed* speed;
	Speed* initSpeed;
	//ArtificialIntelligence* ia;
	list<Tile *>* path;
	int viewRange;

	bool attacking;

	bool hasChanged;
	Timer attackTimer;
	Timer frozenTimer;
	bool frozen;
	float lastAttackingDirection;
private:
	void checkAttackToNewPos(MapData* mapData);
	void lookAtEnemy();

	Entity* attackToEntity;
	list<PlayerEvent*>events;
};

#endif /* MOBILEENTITY_H_ */
