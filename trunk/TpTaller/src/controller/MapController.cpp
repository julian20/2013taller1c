/*
 * MapController.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 */

#include <controller/MapController.h>
#include <iostream>

MapController::MapController(MapView* mapView, MapData* mapData,
		PlayerController* controller) {
	this->mapView = mapView;
	this->mapData = mapData;
	this->screen = mapView->getDrawingSurface();
	this->playerController = controller;
}

void MapController::clickListener(SDL_Event event) {
	if ((event.type == SDL_MOUSEBUTTONDOWN) && (event.button.button == 1)) {
		//this->mapView->IdentifyTile(event.button.x,event.button.y);
		playerController->movePlayer(event.button.x, event.button.y);
	}
}

MapController::~MapController() {
}
