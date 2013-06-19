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
#include <model/map/TileData.h>
#include <view/entities/EntityHolder.h>
#include <model/map/TileData.h>
#include <model/map/MapData.h>
#include <SDL/SDL.h>
#include <map>
#include <vector>
#include <list>

using namespace std;
class EntityViewMap {
public:
	EntityViewMap(MapData* _mapData);
	int getNCols();
	int getNRows();
	void positionEntityView(EntityView* entity, Coordinates coordinates);
	void removeEntityView(EntityView* view);
	void drawViews(SDL_Surface* screen, Position* cam,
			std::map<string, int> visibleTiles);
	list<EntityView*> getListAtRowAndCol(int row, int col);
	void assingEntitiesView(EntityHolder* entityHolder);
	//void drawEntities(TileData* tileData,SDL_Surface* screen, Position* cam, int row, int col);

	virtual ~EntityViewMap();
private:
	bool isEntityInWasVisibleTile(EntityView* entity);

	int rows, cols;
	MapData* mapData;
	EntityHolder* entityHolder;
	vector<vector<list<EntityView*> > > map;
	list<EntityView*> movableEntities;
	std::map<Entity*, bool> addedEntities;
	void updateMovablePos();

};

#endif /* ENTITYVIEWMAP_H_ */
