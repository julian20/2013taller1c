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



typedef enum {MOUSE_TOP,MOUSE_BOTTOM,MOUSE_RIGHT,MOUSE_LEFT,
	MOUSE_CENTER,MOUSE_TOP_RIGHT,MOUSE_TOP_LEFT,MOUSE_BOTTOM_RIGHT,MOUSE_BOTTOM_LEFT} MouseState;

class MapController {
public:
        MapController(MapView* mapView);
        virtual ~MapController();

        void cameraMoveListener();

        bool mouseAtLeft();
        bool mouseAtRight();
        bool mouseAtTop();
        bool mouseAtBottom();
private:
        SDL_Surface* screen;
        MapView* mapView;
        MouseState getMouseState();
        int mouseX, mouseY;
};

#endif /* MAPCONTROLLER_H_ */
