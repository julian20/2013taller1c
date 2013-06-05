/*
 * MultiplayerGame.cpp
 *
 *  Created on: 11/05/2013
 *      Author: tomas
 */

#include <MultiplayerGame.h>
#include <model/map/MapData.h>


using namespace std;

MultiplayerGame::MultiplayerGame(PersistentConfiguration* configuration) {

	MapData* mapData = configuration->getMapData();

	this->gameConfig = configuration->getAnimationConfiguration();
	this->fps = configuration->getAnimationConfiguration()->getFps();
	this->fpsUpdatingTimer = 0;
	this->tempFps = 0;
	Tile::setTileWidth( gameConfig->getTileWidth() );
	Tile::setTileHeight( gameConfig->getTileHeight() );

	EntityViewMap* viewMap = configuration->getEntityViewMap();
	this->view = new MapView(mapData, NULL, viewMap);
	this->mobileEntities = configuration->getMobileEntities();
	for ( map<int,Mob*>::iterator mobs = mobileEntities.begin() ; mobs != mobileEntities.end() ; ++mobs ){
	    ArtificialIntelligence* ia= new ArtificialIntelligence();
	    ia->setEntity(mobs->second);
	    ias.push_back(ia);
	}
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

	NetworkPlayerController* controller = new NetworkPlayerController(player, view->getMapData());
	controllers.insert(pair<string, NetworkPlayerController*>(player->getName(), controller));
	players.push_back(player);

	view->getMapData()->addPlayer(coordiantes->getRow(), coordiantes->getCol(), player);

	Coordinates coords = player->getCoordinates();
	playersCoords[player] = coords;
}

void MultiplayerGame::updateMobs()
{
	MapData* map = view->getMapData();
	for ( list<ArtificialIntelligence*>::iterator ia = ias.begin() ; ia != ias.end() ; ++ia )
	{
		(*ia)->update(map);
	}
}
void MultiplayerGame::updatePlayersCoordinates(){
	Player* player;
	for ( list<Player*>::iterator playerIterator = players.begin() ; playerIterator != players.end() ; ++playerIterator ){
		player = *playerIterator;

		Coordinates initCoords = playersCoords[player];
		Coordinates currentCoords = player->getTile()->getCoordinates();

		if (currentCoords.isEqual( initCoords ))
			continue;

		playersCoords[player] = currentCoords;

		view->getMapData()->updatePlayerPos(initCoords.getRow(), initCoords.getCol(),
											   currentCoords.getRow(), currentCoords.getCol(),
											   player);
	}
}

void MultiplayerGame::playersUpdate(){

	for ( list<Player*>::iterator player = players.begin() ; player != players.end() ; ++player ){
		(*player)->update(view->getMapData());
		(*player)->updateDamageTaken();
	}

	updatePlayersCoordinates();
	updateMobs();
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
		fps = (1000 / elapsedMiliseconds)+5;
}

vector<ChatUpdate*> MultiplayerGame::getChatUpdates() {
	vector<ChatUpdate*> updates;

		for ( list<Player*>::iterator player = players.begin() ; player != players.end() ; ++player )
		{
			ChatUpdate* update = (*player)->generateChatUpdate();
			if (update)
				{
					updates.push_back(update);
				}
		}

		return updates;
}


vector<PlayerUpdate*> MultiplayerGame::getPlayersUpdates(){

	vector<PlayerUpdate*> updates;

	for ( list<Player*>::iterator player = players.begin() ; player != players.end() ; ++player ){
		PlayerUpdate* update = (*player)->generatePlayerUpdate();
		if (update)
			updates.push_back(update);
	}

	return updates;

}


vector<MobUpdate*> MultiplayerGame::getMobileEntitiesUpdates(){

	vector<MobUpdate*> updates;
	for (map<int,Mob*>::iterator it = mobileEntities.begin() ; it != mobileEntities.end() ; ++it){

		MobUpdate* update = (it->second)->generateMobUpdate(it->first);
		if (update)
			updates.push_back(update);
		update->setId(3);
	}

	return updates;

}


list<Player*> MultiplayerGame::getPlayers() {
	return this->players;
}

map<int,MobileEntityInfo*> MultiplayerGame::getMobileEntityInfo(){

	map<int,MobileEntityInfo*> infos;

	for (map<int,Mob*>::iterator it = mobileEntities.begin() ; it != mobileEntities.end() ; ++it){

		MobileEntityInfo* info = new MobileEntityInfo();
		info->setId(3);
		info->setEntity(it->second);
	}

	return infos;

}

MultiplayerGame::~MultiplayerGame() {
	// TODO Auto-generated destructor stub
}



