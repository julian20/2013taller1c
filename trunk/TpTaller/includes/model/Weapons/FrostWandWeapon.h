/*
 * FrostWandWeapon.h
 *
 *  Created on: 11/06/2013
 *      Author: champion
 */

#ifndef FROSTWANDWEAPON_H_
#define FROSTWANDWEAPON_H_
#include <model/Weapons/Weapon.h>

class FrostWandWeapon : public Weapon {
public:
	FrostWandWeapon();
	virtual void attack(Entity& entity);

	virtual ~FrostWandWeapon();
};

#endif /* FROSTWANDWEAPON_H_ */
