/*
 * EntityViewMap.cpp
 *
 *  Created on: 05/04/2013
 *      Author: tomas
 */

#include <view/EntityViewMap.h>

namespace std {

ViewMapIterator::ViewMapIterator(const EntityViewMap* viewMap,Coordinates coordinates){
	int row = coordinates.getRow();
	int col = coordinates.getCol();
	vector< vector< list<EntityView*> > > entities = viewMap->entities;
	entityList = entities.at(col).at(row);
	it = entityList.begin();
}

void ViewMapIterator::next(){
	it++;
}

EntityView* ViewMapIterator::getActual(){
	return *it;
}

bool ViewMapIterator::isAtEnd(){
	return ( it == entityList.end() );
}

ViewMapIterator::~ViewMapIterator(){
}

EntityViewMap::EntityViewMap(int rows, int cols) {

	for (int i = 0 ; i < cols ; i ++){
		list<EntityView*> EntityList;
		vector< list<EntityView*> > row (rows, EntityList);
		entities.push_back(row);
	}
}

void EntityViewMap::positionEntityView (EntityView* entity, Coordinates coordinates){
	int row = coordinates.getRow();
	int col = coordinates.getCol();
	entities.at(col).at(row).push_back(entity);
}

ViewMapIterator* EntityViewMap::getViewMapIterator(Coordinates coordinates){
	return new ViewMapIterator(this,coordinates);
}

int EntityViewMap::getNCols(){
	return entities.size();
}

int EntityViewMap::getNRows(){
	return entities.at(0).size();
}

EntityViewMap::~EntityViewMap() {
	// TODO Auto-generated destructor stub
	// TODO definir si aca se destruyen las EntityViews o no.
}

} /* namespace std */
