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

EntityView* ViewMapIterator::getCurrent(){
	return *it;
}

bool ViewMapIterator::isAtEnd(){
	return ( it == entityList.end() );
}

ViewMapIterator::~ViewMapIterator(){
}

/* ************************************************************************** */
/* ****************            ENTITYVIEWMAP					************* */
/* ************************************************************************** */


EntityViewMap::EntityViewMap(int rows, int cols) {

	for (int i = 0 ; i < cols ; i ++){
		list<EntityView*> EntityList;
		vector< list<EntityView*> > row (rows, EntityList);
		entities.push_back(row);
	}

	this->rows = rows;
	this->cols = cols;
	currentRow = 0;
	currentCol = 0;
	it = new ViewMapIterator(this,Coordinates(currentRow,currentCol));
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
	return cols;
}

int EntityViewMap::getNRows(){
	return rows;
}

void EntityViewMap::initIterator(){
	currentRow = 0;
	currentCol = 0;
	delete it;
	it = new ViewMapIterator(this,Coordinates(currentRow,currentCol));
}

EntityView* EntityViewMap::getNextEntityView(){

	if (!it->isAtEnd()) it->next();

	while (it->isAtEnd()){

		currentCol++;
		if (currentCol == cols){
			currentCol = 0;
			currentRow++;
		}

		if ( iteratorAtEnd() ) return NULL;

		delete it;
		it = new ViewMapIterator(this,Coordinates(currentRow,currentCol));

	}

	return it->getCurrent();
}


list<EntityView*> EntityViewMap::getListAtRowAndCol(int row, int col){

	return entities.at(col).at(row);
}


bool EntityViewMap::iteratorAtEnd(){
	return ((currentCol == cols) || (currentRow == rows));
}

EntityViewMap::~EntityViewMap() {
	delete it;
	// TODO Auto-generated destructor stub
	// TODO definir si aca se destruyen las EntityViews o no.
}

} /* namespace std */
