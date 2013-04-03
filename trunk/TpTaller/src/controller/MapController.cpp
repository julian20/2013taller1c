/*
 * MapController.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 */

#include <controller/MapController.h>
#include <iostream>

#define MovementMargin  50      // px
MapController::MapController(MapView* mapView){
        this->mapView = mapView;
        this->screen = mapView->getDrawingSurface();
}

MouseState MapController::getMouseState() {
	SDL_GetMouseState(&mouseX, &mouseY);
	if (mouseAtLeft() && mouseAtTop()) return MOUSE_TOP_LEFT;
	if (mouseAtRight() && mouseAtTop()) return MOUSE_TOP_RIGHT;
	if (mouseAtLeft() && mouseAtBottom()) return MOUSE_BOTTOM_LEFT;
	if (mouseAtRight() && mouseAtBottom()) return MOUSE_BOTTOM_RIGHT;
	if (mouseAtLeft())return MOUSE_LEFT;
	if (mouseAtRight()) return MOUSE_RIGHT;
	if (mouseAtTop()) return MOUSE_TOP;
	if (mouseAtBottom()) return MOUSE_BOTTOM;


        return MOUSE_CENTER;
}

bool MapController::mouseAtLeft() {
	if (mouseX < MovementMargin) {
			return true;
	}


        return false;
}

bool MapController::mouseAtRight() {

	if (mouseX > screen->w - MovementMargin) {
			return true;
	}
        return false;
}

bool MapController::mouseAtTop() {

	if (mouseY < MovementMargin) {
			return true;
	}

        return false;
}

bool MapController::mouseAtBottom() {
	if (mouseY > screen->h - MovementMargin) {
		return true;
	}


        return false;
}

void MapController::cameraMoveListener(){
	MouseState state = getMouseState();
	switch (state){
	case MOUSE_CENTER:
		break;
	case MOUSE_TOP:
		mapView->moveCamera(MOVE_UP);
		break;
	case MOUSE_BOTTOM:
		mapView->moveCamera(MOVE_DOWN);
		break;
	case MOUSE_LEFT:
		mapView->moveCamera(MOVE_LEFT);
		break;
	case MOUSE_RIGHT:
		mapView->moveCamera(MOVE_RIGHT);
		break;
	case MOUSE_BOTTOM_RIGHT:
		mapView->moveCamera(MOVE_DOWN);
		mapView->moveCamera(MOVE_RIGHT);
		break;
	case MOUSE_BOTTOM_LEFT:
		mapView->moveCamera(MOVE_DOWN);
		mapView->moveCamera(MOVE_LEFT);
		break;
	case MOUSE_TOP_RIGHT:
		mapView->moveCamera(MOVE_UP);
		mapView->moveCamera(MOVE_RIGHT);
		break;
	case MOUSE_TOP_LEFT:
		mapView->moveCamera(MOVE_UP);
		mapView->moveCamera(MOVE_LEFT);
		break;
	}
}


MapController::~MapController() {

}
