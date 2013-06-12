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

#define GOLEM_COST 50

class Golem : public MobileEntity {
public:
	Golem();
	int cost();
	void setOwner(Player* owner);
	void setId(int id);
	int getId();
	virtual void collideTo(MobileEntity& entity);
	void attack(Entity& entity);
	Player* getOwner();
	virtual ~Golem();
private:
	Player* owner;
	int id;
};

#endif /* GOLEM_H_ */
