/*
 * EntityViewMap.cpp
 *
 *  Created on: 05/04/2013
 *      Author: tomas
 */

#include <view/EntityViewMap.h>
#include <model/map/Tile.h>

#include <iostream>
namespace std {

/* ************************************************************************** */
/* ****************            ENTITYVIEWMAP					************* */
/* ************************************************************************** */


EntityViewMap::EntityViewMap(int rows, int cols) {

	for (int i = 0 ; i < cols ; i ++){
		list<EntityView*> EntityList;
		vector< list<EntityView*> > row (rows, EntityList);
		map.push_back(row);
	}

	this->rows = rows;
	this->cols = cols;
}

void EntityViewMap::positionEntityView (EntityView* entity, Coordinates coordinates){
	int row = coordinates.getRow();
	int col = coordinates.getCol();
	map.at(col).at(row).push_back(entity);
	if (entity->isMovable()){
		movableEntities.push_back(entity);
	}
}



int EntityViewMap::getNCols(){
	return cols;
}

int EntityViewMap::getNRows(){
	return rows;
}


list<EntityView*> EntityViewMap::getListAtRowAndCol(int row,int col){
	return map.at(col).at(row);
}

void EntityViewMap::updateMovablePos(){

	for (list<EntityView*>::iterator it = movableEntities.begin() ; it != movableEntities.end() ; ++it){

		EntityView* entityView = *it;
		Entity* entity = entityView->getEntity();

		int initRow = entity->getCoordinates()->getRow();
		int initCol = entity->getCoordinates()->getCol();
		int currentRow = (int) entity->getCurrentPos()->getX();
		int currentCol = (int) entity->getCurrentPos()->getY();
		Coordinates* c = Tile::getTileCoordinates(currentRow,currentCol);
		currentRow = c->getRow();
		currentCol = c->getCol() - 1;

		if (initRow == currentRow && initCol == currentCol) return;

		list<EntityView*> coord = getListAtRowAndCol(initRow, initCol);
		if (!coord.empty()) coord.remove(entityView);
		map[initCol][initRow] = coord;
		map.at(currentCol).at(currentRow).push_back(entityView);
		entity->setCoordinates(currentRow,currentCol);
	}

}

void EntityViewMap::drawViews(SDL_Surface* screen, Position* cam,
		std::map<string, int> visibleTiles){

	updateMovablePos();

	for (int col = visibleTiles["StartCol"]; col < visibleTiles["EndCol"]; col++) {

		for (int row = visibleTiles["StartRow"]; row < visibleTiles["EndRow"]; row++) {

			list<EntityView*>aList = map.at(col).at(row);
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
