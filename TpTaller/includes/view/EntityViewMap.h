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
#include <view/timer/Timer.h>

#include <SDL/SDL.h>

#include <vector>
#include <list>

namespace std {

class EntityViewMap {
public:
	EntityViewMap(int rows, int cols);
	int getNCols();
	int getNRows();
	void positionEntityView (EntityView* entity, Coordinates coordinates);
	void drawViews(SDL_Surface* screen, Position* cam, Timer* timer);
	list<EntityView*> getListAtRowAndCol(int row,int col);
	virtual ~EntityViewMap();
private:
	int rows, cols;
	vector< vector< list<EntityView*> > > map;
	list<EntityView*> movableEntities;

	void updateMovablePos();

};

} /* namespace std */
#endif /* ENTITYVIEWMAP_H_ */
