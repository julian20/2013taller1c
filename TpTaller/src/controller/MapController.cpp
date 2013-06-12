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
		buttonPressed = true;
	}

	if ((event.type == SDL_MOUSEBUTTONUP) && (event.button.button == SDL_BUTTON_LEFT))
		buttonPressed = false;

	if (buttonPressed) {
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

//		if (this->playerController->playerHasclickedAnEntity(mouseX,mouseY)){
//			playerController->playerAttackTo( playerController->getEntityToCollideTo() );
//		}
//		else
	playerController->movePlayer(mouseX, mouseY);

	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_q)) {

		playerController->changeStateUsingShieldSpell();
	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_f)) {
		playerController->frozeEnemies();
	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_e)) {
		playerController->makeEarthquake();
	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_g)) {
			playerController->createGolem();
	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_b)) {
		playerController->changeStateCrystalBall();
	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_LSHIFT)) {
		playerController->toggleRunning();
	}

	if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_LCTRL)) {
		playerController->playerCancelAttack();
	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_LCTRL)) {
		playerController->playerAttack();
	}

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_LALT)) {
		playerController->playerBlock();
	}

	if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_LALT)) {
		playerController->playerCancelBlock();
	}

	if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_TAB)) {
		playerController->playerChangeWeapon();
	}

	if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_SPACE)) {
		Player* player = playerController->getPlayer();
		Vector3* pos = player->getCurrentPos();
		Position* newCameraPos = new Position(pos->getX(), pos->getY());
		mapView->getCamera()->setPosition(newCameraPos);
	}


}

void MapController::generateEventList(bool activated){
	playerController->generateEventList(activated);
}

list<PlayerEvent*> MapController::getEvents(){
	return playerController->getEventList();
}

void MapController::cleanEvents(){
	playerController->cleanEventList();
}

MapController::~MapController() {
}
