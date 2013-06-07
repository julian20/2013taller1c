/*
 * MultiplayerGame.cpp
 *
 *  Created on: 11/05/2013
 *      Author: tomas
 */

#include <MultiplayerGame.h>
#include <model/map/MapData.h>
#include <pthread.h>



using namespace std;

pthread_mutex_t entities_mutex;

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
	vector<MobileEntityView*> viewVector = configuration->getMobileEntityViewList();
	lastAddedView = 0;

	for (unsigned int i = 0 ; i < viewVector.size() ; i++){

		mobileEntities[i] = viewVector[i]->getEntity();
		mobEntView[i] = viewVector[i];
		ArtificialIntelligence* ia= new ArtificialIntelligence();
		ia->setEntity(mobileEntities[i]);
		ias[i] = ia;
		lastAddedView++;

	}
	createFlag(view->getMapData());

}
void MultiplayerGame::createFlag(MapData* mapData)
{
	int cols = mapData->getNCols();
	int rows = mapData->getNRows();
	flag = new Flag();

	int rCol= rand() % cols;
	int rRow= rand() % rows;
	while (mapData->getTileData(rRow,rCol)->getNumberOfEntitiesOnTile()>0)
	{
		rCol= rand() % cols;
		rRow= rand() % rows;
	}
	mapData->addEntity(rRow,rCol,flag);
	flag->setLife(1000);


}

Flag* MultiplayerGame::getFlag() {
	return flag;
}

MenuEvent MultiplayerGame::run(){

	while (true) {
		int ticks = SDL_GetTicks();

		playersUpdate();
		updateMobs();

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
	mobilesCoords[player] = coords;
}

void MultiplayerGame::updateMobs(){
	MapData* mapa = view->getMapData();

	for (map<int,ArtificialIntelligence*>::iterator ia = ias.begin() ; ia != ias.end() ; ++ia )
	{
		(ia->second)->update(mapa);
	}

	updateMobsCoordinates();
}

void MultiplayerGame::updatePlayersCoordinates(){
	Player* player;
	for ( list<Player*>::iterator playerIterator = players.begin() ; playerIterator != players.end() ; ++playerIterator ){
		player = *playerIterator;

		Coordinates initCoords = mobilesCoords[player];
		Coordinates currentCoords = player->getTile()->getCoordinates();

		if (currentCoords.isEqual( initCoords ))
			continue;

		mobilesCoords[player] = currentCoords;

		view->getMapData()->updateMobilePos(initCoords.getRow(), initCoords.getCol(),
											currentCoords.getRow(), currentCoords.getCol(),
											player);
	}
}

void MultiplayerGame::updateMobsCoordinates(){
	MobileEntity* mobileEntity;
	for ( map<int,MobileEntity*>::iterator iter = mobileEntities.begin() ;
			iter != mobileEntities.end() ; ++iter ){
		mobileEntity = (iter->second);

		Coordinates initCoords = mobilesCoords[mobileEntity];
		Coordinates currentCoords = mobileEntity->getTile()->getCoordinates();

		if (currentCoords.isEqual( initCoords ))
			continue;

		mobilesCoords[mobileEntity] = currentCoords;

		view->getMapData()->updateMobilePos(initCoords.getRow(), initCoords.getCol(),
											currentCoords.getRow(), currentCoords.getCol(),
											mobileEntity);
	}
}

void MultiplayerGame::playersUpdate(){

	for ( list<Player*>::iterator player = players.begin() ; player != players.end() ; ++player ){
		(*player)->update(view->getMapData());
		(*player)->updateDamageTaken();
	}

	updatePlayersCoordinates();
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


vector<MobileEntityUpdate*> MultiplayerGame::getMobileEntitiesUpdates(){

	vector<MobileEntityUpdate*> updates;
	for (map<int,MobileEntity*>::iterator it = mobileEntities.begin() ; it != mobileEntities.end() ; ++it){
		MobileEntityUpdate* update = (it->second)->generateMobileEntityUpdate(it->first);
		if (update)
			updates.push_back(update);
	}

	return updates;

}


list<Player*> MultiplayerGame::getPlayers() {
	return this->players;
}

map<int,MobileEntityInfo*> MultiplayerGame::getMobileEntityInfo(){

	map<int,MobileEntityInfo*> infos;

	for (map<int,MobileEntityView*>::iterator it = mobEntView.begin() ; it != mobEntView.end() ; ++it){
		MobileEntityView* view = it->second;
		MobileEntity* ent = view->getEntity();
		MobileEntityInfo* info = new MobileEntityInfo();
		info->setId(it->first);
		info->setName(ent->getName());
		info->setFPS(view->getFps());
		info->setDelay(view->getDelay());
		info->setImageDimentions(view->getImageWidth(),view->getImageHeight());
		info->setAnchorPixel(view->getAnchorPixel());
		info->setEntity(ent);
		info->setImages(view->getTextureHolder()->getMobileEntityImages(info->getName()));
		int col = ent->getCoordinates().getCol();
		int row = ent->getCoordinates().getRow();
		Coordinates* tmp = new Coordinates(row,col);
		info->setInitCoordinates(tmp);
		infos[it->first] = info;
	}

	return infos;

}

vector<int> MultiplayerGame::getDeletedMobileEntities(){
	return deletedMobileEntities;
}

int MultiplayerGame::addMobileEntity(MobileEntityView* view, MobileEntity* entity, Coordinates coordiantes){

	entity->setCoordinates(coordiantes.getRow(), coordiantes.getCol());
	int newId = lastAddedView + 1;
	mobileEntities[newId] = entity;
	mobEntView[newId] = view;
	lastAddedView = newId;


	return newId;
}

void MultiplayerGame::removeMobileEntity(int id){
	if (mobileEntities.count(id) != 0){
		if (ias.count(id) != 0){
			delete ias[id];
			ias.erase(id);
		}
		if (mobEntView.count(id) != 0){
			mobEntView.erase(id);
		}

		mobileEntities.erase(id);

		//TODO: SACARLO TAMBIEN DE MAP DATA

		deletedMobileEntities.push_back(id);
	}


}

MultiplayerGame::~MultiplayerGame() {
	// TODO Auto-generated destructor stub
}



