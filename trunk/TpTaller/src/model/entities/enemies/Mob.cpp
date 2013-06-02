/*
 * Mob.cpp
 *
 *  Created on: May 25, 2013
 *      Author: mint
 */

#include <model/entities/enemies/Mob.h>
using namespace std;

Mob::Mob() {
}

void Mob::setType(MobType type){
	this->type = type;
}

MobType Mob::getType(){
	return this->type;
}

MobUpdate* Mob::generateMobUpdate(int id){
	MobUpdate* update = (MobUpdate*) MobileEntity::generateMobileEntityUpdate(id);
	return update;
}
