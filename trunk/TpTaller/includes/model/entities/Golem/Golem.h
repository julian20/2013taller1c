/*
 * Golem.h
 *
 *  Created on: 09/06/2013
 *      Author: damian
 */

#ifndef GOLEM_H_
#define GOLEM_H_
#include <model/entities/MobileEntity.h>
#include <model/entities/player/Player.h>

class Golem : public MobileEntity {
public:
	Golem();
	int cost();
	void setOwner(Player* owner);
	Player* getOwner();
	virtual ~Golem();
private:
	Player* owner;
};

#endif /* GOLEM_H_ */
