/*
 * Entity.h
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <model/Vector2.h>
#include <model/entityProperties/Position.h>
#include <model/entityProperties/Speed.h>
#include <model/entityProperties/Power.h>
#include <model/entityProperties/Base.h>

#include <string>
#include <vector>
#include <iostream>
using namespace std;
	class Entity {
	public:
		Entity();
		virtual void setPos(float x, float y);
		Vector2* getCurrentPos();
		virtual string getClassName();
		virtual ~Entity();
		Base* getBase();
		void setBase(Base* base);
		std::string getName();
		void setName(std::string name);
	protected:
		Vector2* currentPos;
		Base* base;
		std::string name;
	};

#endif /* ENTITY_H_ */
