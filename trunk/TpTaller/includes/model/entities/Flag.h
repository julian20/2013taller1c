/*
 * Flag.h
 *
 *  Created on: 06/06/2013
 *      Author: damian
 */

#ifndef FLAG_H_
#define FLAG_H_
#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <model/entities/player/Player.h>
#include <string>

using namespace std;
class Flag: public Entity {
public:
	Flag();
	Flag(Entity* entity);
	void attackedBy(string player);
	void collideTo(Player& player);
	void receiveDamage(MobileEntity& player);
	virtual ~Flag();
	string getClassName();
private:
	string player;
	string classname;
};

#endif /* FLAG_H_ */
