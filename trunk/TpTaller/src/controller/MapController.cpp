/*
 * MapController.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 */

#include <controller/MapController.h>
#include <SDL/SDL_mixer.h>
#include <iostream>

MapController::MapController(MapView* mapView, MapData* mapData,
		PlayerController* controller) {
	this->mapView = mapView;
	this->mapData = mapData;
	this->screen = mapView->getDrawingSurface();
	this->playerController = controller;
	this->buttonPressed = false;
}

void MapController::clickListener(SDL_Event event) {
	if ((event.type == SDL_MOUSEBUTTONDOWN) && (event.button.button == 1)) {
		//this->mapView->IdentifyTile(event.button.x,event.button.y);
		//playerController->movePlayer(event.button.x, event.button.y);
		buttonPressed = true;

	/*	Mix_Chunk* darknessVoice = Mix_LoadWAV("tomi.wav");
		if (darknessVoice == NULL) {
			cerr << "No se puede cargar el darknessVoice de darkness"
					<< SDL_GetError() << endl;
			exit(1);
		}*/

		// Establecemos el volumen para el darknessVoice
		//int volumen = 2000;
		//Mix_VolumeChunk(darknessVoice, volumen);

		// Introducimos el sonido en el canal
		// En el canal 1 con una reproducción (1)
		//Mix_PlayChannel(1, darknessVoice, 0);
	}

	if ((event.type == SDL_MOUSEBUTTONUP) && (event.button.button == 1)) buttonPressed = false;

	if (buttonPressed){
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		playerController->movePlayer(mouseX, mouseY);
	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_LSHIFT)){
		playerController->toggleRunning();
	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_LCTRL)){
		playerController->playerAttack();
	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_LALT)){
			playerController->playerBlock();
	}

	if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_LALT)){
				playerController->playerCancelBlock();
		}

}

MapController::~MapController() {
}
