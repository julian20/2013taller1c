/*
 * Entity.h
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <model/Vector2.h>	// Capas hay que volarlo
#include <model/Vector3.h>
#include <model/entityProperties/Coordinates.h>
#include <model/entityProperties/Position.h>
#include <model/entityProperties/Speed.h>
#include <model/entityProperties/Power.h>
#include <model/entityProperties/Base.h>
#include <list>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class Entity {
public:
	Entity();
	void setPos(float x, float y, float z = 0);
	Vector3* getCurrentPos();
	virtual string getClassName();
	virtual ~Entity();
	Base* getBase();
	void setBase(Base* base);
	virtual std::string getName();
	virtual void setName(std::string name);
	Coordinates getCoordinates();
	void setCoordinates(int row, int col);
	void setBaseSizes(int width, int height);
	bool isMobile();
	list<Entity*> getVisibleEnemies();
	virtual int getLife();
protected:
	Vector3* currentPos;
	Base* base;
	std::string name;
	Coordinates* coord;
	int life;
	bool mobile;
	list<Entity*> enemies;
};

#endif /* ENTITY_H_ */
