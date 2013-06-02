/*
 * ArtificialIntelligence.h
 *
 *  Created on: 27/05/2013
 *      Author: damian
 */

#ifndef ARTIFICIALINTELLIGENCE_H_
#define ARTIFICIALINTELLIGENCE_H_

#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <model/map/MapData.h>
#include <list>
class ArtificialIntelligence {
public:
	ArtificialIntelligence();
	void setEntity(Entity* entity);
	virtual ~ArtificialIntelligence();
	void watch();
	void update(MapData* mapData);
	bool isAnyEnemyClose();
	Entity* getNearestEnemy();
	void attack(Entity* enemy);
private:
	Entity* entity;
};

#endif /* ARTIFICIALINTELLIGENCE_H_ */
