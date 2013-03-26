/*
 * MapController.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 */

#include <controller/MapController.h>

#define MovementMargin		50	// px

MapController::MapController() {
	desktop = SDL_GetVideoInfo();
	UpdateMouseState();
}

MapController::~MapController() {

}

void MapController::UpdateMouseState() {
	SDL_GetMouseState(&mouseX, &mouseY);
}

bool MapController::MoveScreenLeft() {
	UpdateMouseState();

	if (mouseX < MovementMargin) {
		if (mouseY > MovementMargin && mouseY < desktop->current_h - MovementMargin) {
			return true;
		}
	}

	return false;
}

bool MapController::MoveScreenRight() {
	UpdateMouseState();

	if (mouseX > desktop->current_w - MovementMargin) {
		if (mouseY > MovementMargin && mouseY < desktop->current_h - MovementMargin) {
			return true;
		}
	}

	return false;
}

bool MapController::MoveScreenUp() {
	UpdateMouseState();

	if (mouseY < MovementMargin) {
		if (mouseX > MovementMargin && mouseX < desktop->current_w - MovementMargin) {
			return true;
		}
	}

	return false;
}

bool MapController::MoveScreenDown() {
	UpdateMouseState();

	if (mouseY > desktop->current_h - MovementMargin) {
		if (mouseX > MovementMargin && mouseX < desktop->current_w - MovementMargin) {
			return true;
		}
	}

	return false;
}

bool MapController::MoveScreenLeftUp() {
	UpdateMouseState();

	if (mouseX < MovementMargin) {
		if (mouseY < MovementMargin) {
			return true;
		}
	}

	return false;
}

bool MapController::MoveScreenLeftDown() {
	UpdateMouseState();

	if (mouseX < MovementMargin) {
		if (mouseY > desktop->current_h - MovementMargin) {
			return true;
		}
	}

	return false;
}

bool MapController::MoveScreenRightUp() {
	UpdateMouseState();

	if (mouseX >  desktop->current_w - MovementMargin) {
		if (mouseY < MovementMargin) {
			return true;
		}
	}

	return false;
}

bool MapController::MoveScreenRightDown() {
	UpdateMouseState();

	if (mouseX >  desktop->current_w - MovementMargin) {
		if (mouseY > desktop->current_h - MovementMargin) {
			return true;
		}
	}

	return false;
}


