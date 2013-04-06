/*
 * EntityViewMap.h
 *
 *  Created on: 05/04/2013
 *      Author: tomas
 */

#ifndef ENTITYVIEWMAP_H_
#define ENTITYVIEWMAP_H_

#include <view/entities/EntityView.h>
#include <model/entityProperties/Coordinates.h>

#include <vector>
#include <list>

namespace std {

class EntityViewMap;

class ViewMapIterator {
public:
	ViewMapIterator(const EntityViewMap* viewMap,Coordinates coordinates);
	bool isAtEnd();
	EntityView* getActual();
	void next();
	virtual ~ViewMapIterator();
private:
	list<EntityView*> entityList;
	list<EntityView*>::iterator it;
};

class EntityViewMap {
public:
	EntityViewMap(int rows, int cols);
	int getNCols();
	int getNRows();
	void positionEntityView (EntityView* entity, Coordinates coordinates);
	 ViewMapIterator* getViewMapIterator(Coordinates coordinates);
	virtual ~EntityViewMap();
private:
	friend class ViewMapIterator;
	vector< vector< list<EntityView*> > > entities;
};

} /* namespace std */
#endif /* ENTITYVIEWMAP_H_ */
