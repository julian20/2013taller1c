/*
 * mapController.h
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 */

#ifndef MAPCONTROLLER_H_
#define MAPCONTROLLER_H_

#include <view/MapView.h>
#include <SDL/SDL.h>





class MapController {
public:
        MapController(MapView* mapView,MapData* mapData);
        virtual ~MapController();


        void clickListener(SDL_Event event);

private:
        MapData* mapData;
        SDL_Surface* screen;
        MapView* mapView;
};

#endif /* MAPCONTROLLER_H_ */
