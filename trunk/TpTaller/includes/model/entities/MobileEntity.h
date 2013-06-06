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

#define ATTACK_TIMEOUT 1000
using namespace std;
class MapData;
class Item;
class LifeHeart;
class SpellEffect;

class MobileEntity: public Entity {
public:
	MobileEntity();
	MobileEntity(string name, Position* position, Speed* speed);
	virtual ~MobileEntity();
	void moveTo(int x, int y, int z = 0);
	void setPos(float x, float y, float z = 0);

	virtual void update(MapData* mapData);
	void updateFromServer(MobileEntityUpdate* update);
	MobileEntityUpdate* generateMobileEntityUpdate(int id);

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

	void castSpell();
	void attackTo(Entity* attackTo);
	void attack();
	void setAttack(bool attacking);
	bool isAttacking();
	void cancelAttack();
	void setCastingSpell(bool castingSpell);
	bool isCastingSpell();

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
	SpellEffect* getCurrentSpell();
protected:
	void loadNextPosition();
	void emptyPath();
	Vector3* endPos;
	Speed* speed;
	Speed* initSpeed;
	//ArtificialIntelligence* ia;
	list<Tile *>* path;
	list<SpellEffect*>* spellEffects;

	bool attacking;

	bool hasChanged;
	Timer attackTimer;
	float lastAttackingDirection;
	bool castingSpell;
private:
	void checkAttackToNewPos(MapData* mapData);
	void lookAtEnemy();
	void castSpellNow(MapData* mapData);
	void setSpellDirection(SpellEffect* spell,
			Coordinates starting, Coordinates ending);

	bool needCastSpell;
	Entity* attackToEntity;
	list<PlayerEvent*>events;
};

#endif /* MOBILEENTITY_H_ */
