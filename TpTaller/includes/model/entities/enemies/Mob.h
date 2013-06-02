/*
 * Mob.h
 *
 *  Created on: May 25, 2013
 *      Author: mint
 */

#ifndef Mob_H_
#define Mob_H_

#include <stdio.h>
#include <iostream>
#include <list>
#include <model/map/Tile.h>
#include <model/entities/Entity.h>
#include <model/Vector2.h>
#include <model/Vector3.h>
#include <model/entities/MobileEntity.h>
#include <networking/MobUpdate.h>

using namespace std;

typedef enum { TYPE_ZOMBIE, TYPE_GOLEM } MobType;

class Mob: public MobileEntity {
public:
	Mob();
	void setType(MobType type);
	MobType getType();
	MobUpdate* generateMobUpdate(int id);

protected:

private:
	MobType type;
};

#endif /* Mob_H_ */
