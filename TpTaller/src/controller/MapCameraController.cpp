/*
 * MapCameraController.cpp
 *
 *  Created on: 07/04/2013
 *      Author: tomas
 */

#include <controller/MapCameraController.h>

namespace std {

MapCameraController::MapCameraController(MapCameraView* camera, unsigned int movementMargin) {
	this->camera = camera;
	this->screen = camera->getActiveScreen();
	this->movementMargin = movementMargin;
	SDL_GetMouseState(&mouseX, &mouseY);
}

MousePosition MapCameraController::getMousePosition() {

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

int MapCameraController::computeMovementSpeed(MousePosition pos){

	int speed = 0;

	switch (pos){
	case MOUSE_CENTER:
		speed = 0;
		break;
	case MOUSE_LEFT:
		speed = (movementMargin - mouseX)/2;
		break;
	case MOUSE_RIGHT:
		speed = (movementMargin + (mouseX - screen->w))/2;
		break;
	case MOUSE_TOP:
		speed = (movementMargin - mouseY)/2;
		break;
	case MOUSE_BOTTOM:
		speed = (movementMargin + (mouseY - screen->h))/2;
		break;
	case MOUSE_TOP_LEFT:
		speed = (movementMargin - mouseY)/4 + (movementMargin - mouseX)/4;
		break;
	case MOUSE_TOP_RIGHT:
		speed = (movementMargin - mouseY)/4 + (movementMargin + (mouseX - screen->w))/4;
		break;
	case MOUSE_BOTTOM_LEFT:
		speed = (movementMargin + (mouseY - screen->h))/4 + (movementMargin - mouseX)/4;
		break;
	case MOUSE_BOTTOM_RIGHT:
		speed = (movementMargin + (mouseY - screen->h))/4 + (movementMargin + (mouseX - screen->w))/4;
		break;
	}


	return speed;

}


bool MapCameraController::mouseAtLeft() {
	if (mouseX < movementMargin) {
			return true;
	}


        return false;
}

bool MapCameraController::mouseAtRight() {

	if (mouseX > screen->w - movementMargin) {
			return true;
	}
        return false;
}

bool MapCameraController::mouseAtTop() {

	if (mouseY < movementMargin) {
			return true;
	}

        return false;
}

bool MapCameraController::mouseAtBottom() {
	if (mouseY > screen->h - movementMargin) {
		return true;
	}


        return false;
}

void MapCameraController::cameraMoveListener(){
	MousePosition state = getMousePosition();
	int speed = computeMovementSpeed(state);
	switch (state){
	case MOUSE_CENTER:
		break;
	case MOUSE_TOP:
		camera->moveCamera(MOVE_UP,speed);
		break;
	case MOUSE_BOTTOM:
		camera->moveCamera(MOVE_DOWN,speed);
		break;
	case MOUSE_LEFT:
		camera->moveCamera(MOVE_LEFT,speed);
		break;
	case MOUSE_RIGHT:
		camera->moveCamera(MOVE_RIGHT,speed);
		break;
	case MOUSE_BOTTOM_RIGHT:
		camera->moveCamera(MOVE_DOWN,speed);
		camera->moveCamera(MOVE_RIGHT,speed);
		break;
	case MOUSE_BOTTOM_LEFT:
		camera->moveCamera(MOVE_DOWN,speed);
		camera->moveCamera(MOVE_LEFT,speed);
		break;
	case MOUSE_TOP_RIGHT:
		camera->moveCamera(MOVE_UP,speed);
		camera->moveCamera(MOVE_RIGHT,speed);
		break;
	case MOUSE_TOP_LEFT:
		camera->moveCamera(MOVE_UP,speed);
		camera->moveCamera(MOVE_LEFT,speed);
		break;
	}
}

MapCameraController::~MapCameraController() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
