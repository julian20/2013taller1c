/*
 * MultiplayerGame.cpp
 *
 *  Created on: 11/05/2013
 *      Author: tomas
 */

#include <MultiplayerGame.h>
#include <model/map/MapData.h>


namespace std {

MultiplayerGame::MultiplayerGame(PersistentConfiguration* configuration) {

	MapData* mapData = configuration->getMapData();

	this->gameConfig = configuration->getAnimationConfiguration();
	this->fps = 0;
	this->fpsUpdatingTimer = 0;
	this->tempFps = 0;
	Tile::setTileWidth( gameConfig->getTileWidth() );
	Tile::setTileHeight( gameConfig->getTileHeight() );

	EntityViewMap* viewMap = configuration->getEntityViewMap();
	this->view = new MapView(mapData, NULL, viewMap);

}


MenuEvent MultiplayerGame::run(){

	while (true) {
		int ticks = SDL_GetTicks();

		playersUpdate();

		applyFPS(ticks);
	}

	return EXIT_EVENT;

}

void MultiplayerGame::addEventsToHandle(string playerName, vector<PlayerEvent*> events){

	controllers[playerName]->handleEvents(events);

}

void MultiplayerGame::addNewPlayer(Player* player, Coordinates* coordiantes){

	NetworkPlayerController* controller = new NetworkPlayerController(player,view->getMapData());
	controllers.insert(pair<string, NetworkPlayerController*>(player->getName(),controller));
	players.push_back(player);

}

void MultiplayerGame::playersUpdate(){

	for ( list<Player*>::iterator player = players.begin() ; player != players.end() ; ++player ){
		(*player)->update();
	}

}

void MultiplayerGame::applyFPS(int timer) {
	timer = SDL_GetTicks() - timer;
	float elapsedMiliseconds = timer;
	unsigned int FPS = this->gameConfig->getFps();

	float delay = (float) 1000 / FPS;

	if (delay - elapsedMiliseconds > 0) {
		SDL_Delay(delay - elapsedMiliseconds);
		fps = (float) 1000 / delay;

	} else
		fps = 1000 / elapsedMiliseconds;
}

vector<PlayerUpdate*> MultiplayerGame::getPlayersUpdates(){

	vector<PlayerUpdate*> updates;

	for ( list<Player*>::iterator player = players.begin() ; player != players.end() ; ++player ){
		PlayerUpdate* update = (*player)->generatePlayerUpdate();
		if (update)
			{
			updates.push_back(update);
			}
	}

	return updates;

}

MultiplayerGame::~MultiplayerGame() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */


