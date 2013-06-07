/*
 * Flag.h
 *
 *  Created on: 06/06/2013
 *      Author: damian
 */

#ifndef FLAG_H_
#define FLAG_H_
#include <model/entities/Entity.h>
#include <model/entities/player/Player.h>
#include <string>

using namespace std;
class Flag : public MobileEntity {
public:
	Flag();
	void attackedBy(string player);
	void collideTo(MobileEntity& player);
	void receiveDamage(MobileEntity& player);
	virtual ~Flag();
	string getClassName();
private:
	string player;
	string name;
};

#endif /* FLAG_H_ */
