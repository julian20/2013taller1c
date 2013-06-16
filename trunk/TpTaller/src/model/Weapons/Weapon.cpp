/*
 * Weapon.cpp
 *
 *  Created on: 01/06/2013
 *      Author: damian
 */

#include <model/Weapons/Weapon.h>
#define MAXACCURACY 10
Weapon::Weapon() {
	range = 0;
	damage = 0;
	accuracy = 0;
	magic = 0;
	team = -1;
	uses = -1;	// Para cantidad de usos infinito
}

string Weapon::getName() {
	return name;
}

void Weapon::setRange(int range) {
	this->range = range;
}

void Weapon::setDamage(int damage) {
	this->damage = damage;
}

int Weapon::getDamage() {
	return this->damage;
}

int Weapon::getAccuracy() {
	return this->accuracy;
}

int Weapon::getRange() {
	return this->range;
}

int Weapon::getMagic() {
	return this->magic;
}

void Weapon::setAccuracy(int accuracy) {
	if (accuracy > MAXACCURACY)
		this->accuracy = MAXACCURACY;
	else
		this->accuracy = accuracy;
}

void Weapon::setMagic(int magic) {
	this->magic = magic;
}

void Weapon::attack(Entity& enemy) {
	this->applyDamage(enemy);
}

void Weapon::setUses(int uses) {
	this->uses = uses;
}

int Weapon::getUses() {
	return uses;
}

void Weapon::setTeam(int team) {
	this->team = team;
}

int Weapon::getTeam() {
	return this->team;
}

void Weapon::applyDamage(Entity& entity) {
	int randomAccuracy = rand() % MAXACCURACY + 1;
	int damageToApply = ceil(
			(float(this->accuracy - randomAccuracy) / MAXACCURACY) * damage);

	// Casteo a puntero.
	Entity* entityPtr = (Entity*) &entity;
	entityPtr->applyDamage(damageToApply);

	if ((entityPtr->getLife() + entityPtr->getShield() - damageToApply) <= 0) {
		entityPtr->setKilledBy(team);
	}

}

bool Weapon::canAttackTo(string name) {
	list<string>::const_iterator iter;
	for (iter = notValidAttacksNames.begin(); iter != notValidAttacksNames.end(); ++iter) {
		string current = *iter;

		if (name.compare(current) == 0)
			return false;
	}

	return true;
}

Weapon::~Weapon() {
}

