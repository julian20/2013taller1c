/*
 * Weapon.cpp
 *
 *  Created on: 01/06/2013
 *      Author: damian
 */

#include <model/Weapons/Weapon.h>
#define MAXACCURACY 10
Weapon::Weapon() {
	range=0;
	damage=0;
	accuracy=0;
}
void Weapon::setRange(int range)
{
	this->range=range;
}
void Weapon::setDamage(int damage)
{
	this->damage=damage;
}
void Weapon::setAccuracy(int accuracy)
{
	if(accuracy> MAXACCURACY) this->accuracy=MAXACCURACY;
	else this->accuracy=accuracy;
}
void Weapon::applyDamage(Entity* entity)
{
	int randomAccuracy = rand() % MAXACCURACY+1;
	int damageToApply = ((this->accuracy-randomAccuracy)/MAXACCURACY)*damage;
	entity->applyDamage(damageToApply);
}
void Weapon::attack(Entity* enemy)
{
	this->applyDamage(enemy);
}

Weapon::~Weapon() {
	// TODO Auto-generated destructor stub
}

