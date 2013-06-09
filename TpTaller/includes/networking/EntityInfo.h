/*
 * EntityInfo.h
 *
 *  Created on: 08/06/2013
 *      Author: tomas
 */

#ifndef ENTITYINFO_H_
#define ENTITYINFO_H_

#include <model/Vector2.h>
#include <model/Vector3.h>
#include <model/entities/Entity.h>
#include <view/entities/EntityView.h>
#include <view/entities/ItemViewHolder.h>

#include <string>

using namespace std;

class EntityInfo {
public:
	EntityInfo();

	void setId(int id);
	int getId();

	void setName(string name);
	string getName();

	void setInitCoordinates(Coordinates* coords);
	Coordinates* getInitCoordinates();


	void setClassName(string name);
	string getClassName();


	void setEntity(Entity* entity);
	Entity* getEntity();

	EntityView* createEntityView(ItemViewHolder* vh);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const EntityInfo&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, EntityInfo&);

	virtual ~EntityInfo();
private:
	int id;
	string name;
	string className;

	Coordinates* initCoords;



	Entity* entity;
};

#endif
