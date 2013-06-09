/*
 * Golem.h
 *
 *  Created on: 09/06/2013
 *      Author: damian
 */

#ifndef GOLEM_H_
#define GOLEM_H_
#include <model/entities/MobileEntity.h>
class Golem : public MobileEntity {
public:
	Golem();
	int cost();
	virtual ~Golem();
};

#endif /* GOLEM_H_ */
