/*
 * Mob.cpp
 *
 *  Created on: May 25, 2013
 *      Author: mint
 */

#include <model/entities/enemies/Mob.h>
using namespace std;


#define MOB_ATTACK_TIMEOUT 3000
Mob::Mob() : MobileEntity() {
	this->weapons = new list<Weapon*>;
	Sword* sword = new Sword();
	sword->setAccuracy(10);
	sword->setDamage(10);
	sword->setRange(1);
	sword->setMagic(0);
	weapons->push_front(sword);
	type = TYPE_ZOMBIE;
}

void Mob::setType(MobType type) {
	this->type = type;
}

void Mob::collideTo(Entity& entity) {
	attack(entity);
}

void Mob::reverseCollide(Entity& entity) {
	entity.collideTo(*this);
}
Weapon* Mob::getCurrentWeapon() {
	Weapon* weaponToUse = weapons->front();
	return weaponToUse;
}

void Mob::attack(Entity& entity) {
	if (attackTimer.getTimeIntervalSinceStart() > MOB_ATTACK_TIMEOUT) {
		attacking = true;
		attackTimer.start();
		//this->magic--;
		if (magic > 0) {
			Weapon* weaponToUse = this->getCurrentWeapon();
			weaponToUse->attack(entity);
			this->magic -= weaponToUse->getMagic();
		}
	}

}

MobType Mob::getType() {
	return this->type;
}

MobUpdate* Mob::generateMobUpdate(int id) {
	MobUpdate* update = (MobUpdate*) MobileEntity::generateMobileEntityUpdate(
			id);
	return update;
}
