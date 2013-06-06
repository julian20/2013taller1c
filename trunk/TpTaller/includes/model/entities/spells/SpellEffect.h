/*
 * SpellEffect.h
 *
 *  Created on: Jun 5, 2013
 *      Author: mint
 */

#ifndef SPELLEFFECT_H_
#define SPELLEFFECT_H_
#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <model/entityProperties/Position.h>

class SpellEffect: public MobileEntity {
public:
	SpellEffect();
	virtual void applyEffects(Entity& entity);
	virtual void applyEffects(MobileEntity& entity);
	virtual void collideTo(MobileEntity& entity);
	virtual string getClassName();
	void setUp();
	void update(MapData* mapData);

	void setDirection(Vector3 direction);
	Vector3 getDirection();
	void setMaxTraveledTiles(int maxTraveledTiles);
	int getMaxTraveledTiles();

	Vector3 getLEFT();
	Vector3 getUP_LEFT();
	Vector3 getUP();
	Vector3 getUP_RIGHT();
	Vector3 getRIGHT();
	Vector3 getDOWN_RIGHT();
	Vector3 getDOWN();
	Vector3 getDOWN_LEFT();

	virtual ~SpellEffect();
protected:
	int maxTraveledTiles;
	Vector3 direction;
	Coordinates startingCoords;
private:
	bool reachMaxDistance();
	void defineConstants();

	Vector3 LEFT, UP_LEFT, UP, UP_RIGHT, RIGHT,
	DOWN_RIGHT, DOWN, DOWN_LEFT;
};

#endif /* SPELLEFFECT_H_ */
