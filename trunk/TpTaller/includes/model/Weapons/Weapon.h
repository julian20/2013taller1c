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
	void applyDamage(Entity* entity);
	void attack(Entity* entity);
	void setDamage(int damage);
	void setAccuracy(int accuracy);
	void setRange(int range);

protected:
	int damage;
	int range;
	int accuracy;
	string name;
};

#endif /* WEAPON_H_ */
