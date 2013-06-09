/*
 * Entity.h
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <model/Vector2.h>	// Capas hay que volarlo
#include <model/Vector3.h>
#include <model/map/Tile.h>
#include <model/entityProperties/Coordinates.h>
#include <model/entityProperties/Position.h>
#include <model/entityProperties/Speed.h>
#include <model/entityProperties/Power.h>
#include <model/entityProperties/Base.h>
#include <view/timer/Timer.h>
#include <list>
#include <string>
#include <vector>
#include <iostream>

class MobileEntity;
class Player;
class Item;
class LifeHeart;

using namespace std;
class Entity {
public:
	Entity();
	Entity(Entity* entity);
	void setPos(float x, float y, float z = 0);
	Vector3* getCurrentPos();
	virtual string getClassName();
	virtual void applyDamage(int damage);
	void reduceMagic(int quantity);
	virtual ~Entity();
	Base* getBase();
	void setBase(Base* base);
	virtual std::string getName();
	virtual void setName(std::string name);
	Coordinates getCoordinates();
	void setCoordinates(int row, int col);
	void setBaseSizes(int width, int height);
	bool isWalkable();
	bool isAttackable();
	bool getHideInFog();

	void setRemoveFromGame(bool removeFromGame);
	bool getRemoveFromGame();

	void setShield(int shield);
	int getShield();

	list<Entity*> getVisibleEnemies();
	virtual int getLife();
	virtual int getMagic();
	virtual void setLife(int life);
	virtual void setMagic(int magic);
	void setTile(Tile* _tile);
	virtual Tile* getTile();

	virtual void collideTo(MobileEntity& entity);
	virtual void collideTo(Player& entity);

	void resetRespawnTimer();

	// Mission manager.
	bool isDead();
	int getTeam();
	int getKilledBy();
	void setTeam(int team);
	void setKilledBy(int team);
	bool getPositionInitialized();
	void updateDamageTaken();
	void setDamageBuffer(int dmg);
	int getDamageBuffer();
protected:
	Vector3* currentPos;
	Base* base;
	std::string name;
	Coordinates* coord;
	list<Entity*> enemies;
	Tile* currentTile;
	int life;
	int magic;
	bool walkable;
	bool attackable;
	int shield;

	// Mission manager.
	int team;
	int killedBy;
	Timer damageTimer;
	Timer magicDamageDelay;
	Timer respawnTimer;
	int damageBuffer;

	bool removeFromGame;
	bool hideInFog;
private:
	bool positionInitialized;
};

#endif /* ENTITY_H_ */
