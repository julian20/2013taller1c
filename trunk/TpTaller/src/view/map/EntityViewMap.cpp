/*
 * EntityViewMap.cpp
 *
 *  Created on: 05/04/2013
 *      Author: tomas
 */

#include <view/EntityViewMap.h>
#include <model/map/Tile.h>
#include <string>

#include <iostream>

using namespace std;
/* ************************************************************************** */
/* ****************            ENTITYVIEWMAP					************* */
/* ************************************************************************** */

EntityViewMap::EntityViewMap(MapData* _mapData) {
	this->mapData = _mapData;
	this->rows = mapData->getNRows();
	this->cols = mapData->getNCols();

	for (int i = 0; i < cols; i++) {
		list<EntityView*> EntityList;
		vector<list<EntityView*> > row(rows, EntityList);
		map.push_back(row);
	}
}

void EntityViewMap::positionEntityView(EntityView* entity,
		Coordinates coordinates) {
	int row = coordinates.getRow();
	int col = coordinates.getCol();

	map.at(col).at(row).push_back(entity);
	if (entity->isMovable()) {
		movableEntities.push_back(entity);
	}
}

int EntityViewMap::getNCols() {
	return cols;
}

int EntityViewMap::getNRows() {
	return rows;
}

list<EntityView*> EntityViewMap::getListAtRowAndCol(int row, int col) {
	return map.at(col).at(row);
}

void EntityViewMap::updateMovablePos() {

	for (list<EntityView*>::iterator it = movableEntities.begin();
			it != movableEntities.end(); ++it) {

		EntityView* entityView = *it;
		Entity* entity = entityView->getEntity();

		int initRow = entity->getCoordinates()->getRow();
		int initCol = entity->getCoordinates()->getCol();
		Vector3* entityPos = entity->getCurrentPos();
		Coordinates* c = Tile::getTileCoordinates(entityPos->getX(),
				entityPos->getY());
		int currentRow = c->getRow();
		int currentCol = c->getCol();

		if (initRow == currentRow && initCol == currentCol)
			return;

		list<EntityView*> coord = getListAtRowAndCol(initRow, initCol);
		if (!coord.empty())
			coord.remove(entityView);
		map[initCol][initRow] = coord;
		entity->setCoordinates(currentRow, currentCol);
		map.at(currentCol).at(currentRow).push_back(entityView);

		mapData->updatePersonajePos(initRow, initCol, currentRow, currentCol,
															(Player*) entity);
	}

}

void EntityViewMap::assingEntitiesView(EntityHolder* entityHolder) {
	this->entityHolder = entityHolder;
}
/*void EntityViewMap::drawEntities(TileData* tileData,SDL_Surface* screen, Position* cam, int row, int col)
 {
 for(int i=0; i<tileData->getNumberOfEntitiesOnTile();i++)
 {
 Entity* entity= tileData->getNextEntity();
 EntityViewData* viewData= entityHolder->getEntity(entity->getName());
 SDL_Surface* imageEntity=viewData->getEntityImage();
 //Se la paso a una entityView y listo singleton.
 SDL_Rect clip;
 clip.x = viewData->getImageWidth() * 1 * 1;
 clip.y = 0;
 clip.w = viewData->getImageWidth() ;
 clip.h = viewData->getImageHeight() ;
 Vector3* position = entity->getCurrentPos();
 int x = (int) position->getX();
 int y = (int) position->getY();

 SDL_Rect offset;
 offset.x = (int) (Tile::computePositionTile(x, y, true).x + cam->getX() - (int)viewData->getImageWidth()/ 2);
 offset.y = (int) (Tile::computePositionTile(x, y, true).y + cam->getY() - (int)viewData->getImageHeight() / 2);
 offset.h = clip.h;
 offset.w = clip.w;

 SDL_BlitSurface(imageEntity, &clip, screen, &offset);
 }
 }*/
void EntityViewMap::drawViews(SDL_Surface* screen, Position* cam,
		std::map<string, int> visibleTiles) {

	updateMovablePos();

	for (int col = visibleTiles["StartCol"]; col < visibleTiles["EndCol"];
			col++) {

		for (int row = visibleTiles["StartRow"]; row < visibleTiles["EndRow"];
				row++) {

			TileData* tileData = mapData->getTileData(row, col);

			if (tileData->getWasVisible()) {
				list<EntityView*> aList = map.at(col).at(row);
				if (!aList.empty()) {
					list<EntityView*>::iterator it;
					for (it = aList.begin(); it != aList.end(); ++it) {
						EntityView* view = *it;
						if (!view)
							continue;
						bool drawFog = !(tileData->getIsVisible());
						view->draw(screen, cam, drawFog);
					}
				}
			}
		}

	}

}

EntityViewMap::~EntityViewMap() {

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			list<EntityView*> aList = map.at(i).at(j);
			if (!aList.empty()) {
				list<EntityView*>::iterator it;
				for (it = aList.begin(); it != aList.end(); ++it) {
					EntityView* view = *it;
					if (!view)
						continue;
					delete view;
				}
			}
		}
	}
}
