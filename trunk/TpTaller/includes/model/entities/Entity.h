/*
 * Entity.h
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <model/entityProperties/position/Position.h>
#include <model/entityProperties/speed/Speed.h>
#include <string>

class Entity {
public:
	Entity(std::string name, Position* position, Speed* speed);
	virtual ~Entity();
	const Position* getPosition() const;
	void setPosition(Position* position);
	const Speed* getSpeed() const;
	void setSpeed(Speed* speed);
	const std::string getName() const;
	void setName(std::string name);

private:
	Position* position;
	Speed* speed;
	std::string name;
	Entity();
};

#endif /* ENTITY_H_ */
