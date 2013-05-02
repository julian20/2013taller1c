/*
 * mapController.h
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 */

#ifndef MAPCONTROLLER_H_
#define MAPCONTROLLER_H_

#include <view/MapView.h>
#include <controller/PlayerController.h>

#include <SDL/SDL.h>

class MapController {
public:
	MapController(MapView* mapView, MapData* mapData,
			PlayerController* controller);
	virtual ~MapController();
	void clickListener(SDL_Event event);
	void generateEventList(bool activated);
	list<PlayerEvent*> getEvents();
	void cleanEvents();

private:
	MapData* mapData;
	SDL_Surface* screen;
	MapView* mapView;
	PlayerController* playerController;
	bool buttonPressed;
};

#endif /* MAPCONTROLLER_H_ */
