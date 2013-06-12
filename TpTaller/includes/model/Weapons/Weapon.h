/*
 * Weapon.h
 *
 *  Created on: 01/06/2013
 *      Author: damian
 */

#ifndef WEAPON_H_
#define WEAPON_H_
#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <string>
using namespace std;
class Weapon {
public:
	Weapon();
	void applyDamage(Entity& entity);
	void reduceEnergy(Entity& entity);
	virtual void attack(Entity& entity);
	string getName();
	void setDamage(int damage);
	void setAccuracy(int accuracy);
	void setRange(int range);
	void setMagic(int magic);
	void setTeam(int team);
	int getDamage();
	int getRange();
	int getAccuracy();
	int getMagic();
	int getTeam();

	virtual ~Weapon();
protected:
	int damage;
	int magic;
	int range;
	int accuracy;
	int team;
	string name;
};

#endif /* WEAPON_H_ */
