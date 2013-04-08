/*
 * EntityViewMap.cpp
 *
 *  Created on: 05/04/2013
 *      Author: tomas
 */

#include <view/EntityViewMap.h>

#include <iostream>

namespace std {

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
}

void EntityViewMap::positionEntityView (EntityView* entity, Coordinates coordinates){
	int row = coordinates.getRow();
	int col = coordinates.getCol();
	entities.at(col).at(row).push_back(entity);
}



int EntityViewMap::getNCols(){
	return cols;
}

int EntityViewMap::getNRows(){
	return rows;
}


list<EntityView*> EntityViewMap::getListAtRowAndCol(int row,int col){
	return entities.at(col).at(row);
}

void EntityViewMap::drawViews(SDL_Surface* screen, Position* cam){

	for (int col = 0; col < cols; col++) {

		for (int row = 0; row < rows; row++) {


			list<EntityView*>aList = entities.at(col).at(row);
			if(!aList.empty()){
				list<EntityView*>::iterator it;
				for (it = aList.begin(); it != aList.end(); ++it) {
					EntityView* view = *it;
					if (!view) continue;
					view->draw(screen,cam);
				}
			}
		}

	}

}


EntityViewMap::~EntityViewMap() {
}

} /* namespace std */
