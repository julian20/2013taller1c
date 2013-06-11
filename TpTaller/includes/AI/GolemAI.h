/*
 * GolemAI.h
 *
 *  Created on: 11/06/2013
 *      Author: tomas
 */

#ifndef GOLEMAI_H_
#define GOLEMAI_H_

#include <AI/ArtificialIntelligence.h>

class GolemAI : public ArtificialIntelligence {
public:
	GolemAI();
	virtual void update(MapData* mapData);
	virtual ~GolemAI();
};

#endif /* GOLEMAI_H_ */
