/*
 * Weapon.h
 *
 *  Created on: 01/06/2013
 *      Author: damian
 */

#ifndef WEAPON_H_
#define WEAPON_H_
#include <model/entities/Entity.h>
#include <string>
using namespace std;
class Weapon {
public:
	Weapon();
	virtual ~Weapon();
	void applyDamage(Entity& entity);
	void reduceEnergy(Entity& entity);
	void attack(Entity& entity);
	void setDamage(int damage);
	void setAccuracy(int accuracy);
	void setRange(int range);
	void setMagic(int magic);
	int getDamage();
	int getRange();
	int getAccuracy();
	int getMagic();

protected:
	int damage;
	int magic;
	int range;
	int accuracy;
	string name;
};

#endif /* WEAPON_H_ */
