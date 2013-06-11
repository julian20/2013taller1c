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

	vector<Item*> itemVector = configuration->getItems();
	vector<MobileEntity*> mobileEntityVector = configuration->getMobileEntityList();

	EntityViewMap* viewMap = configuration->getEntityViewMap();
	this->view = new MapView(mapData, NULL, viewMap);
	lastAddedView = 0;

	for (unsigned int i = 0 ; i < mobileEntityVector.size() ; i++){
		ArtificialIntelligence* ia= new ArtificialIntelligence();
		ia->setMobileEntity(mobileEntityVector[i]);
		ias[i] = ia;
		addMobileEntity(mobileEntityVector[i],mobileEntityVector[i]->getCoordinates());
	}

	for (unsigned int i = 0 ; i < itemVector.size() ; i++){
			addEntity(itemVector[i],itemVector[i]->getCoordinates());
	}

	createFlag(view->getMapData());
	flag = configuration->getFlag();

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

Entity* MultiplayerGame::getFlag() {
	return flag;
}
void MultiplayerGame::createGolem(Player* player)
{
	Golem* golem = new Golem();
	Coordinates coor = player->getCoordinates();
	golem->setCoordinates(coor.getRow()+1,coor.getCol());
	golem->setTeam(player->getTeam());
	golem->setName("dragon");
	golem->setSpeed(player->getSpeed());
	this->createGolemIa(golem);
	golemsMap[player->getName()] = golem ;
	addMobileEntity(golem,coor);

}
void MultiplayerGame::createGolemIa(MobileEntity* golem)
{
	ArtificialIntelligence* ia = new ArtificialIntelligence();
	ia->setMobileEntity(golem);
	int cant = ias.size();
	this->ias[cant++] = ia ;
}
MenuEvent MultiplayerGame::run(){
	view->getMapData()->cleanNewEntities();

	while (true) {
		view->getMapData()->cleanNewMobileEntities();

		int ticks = SDL_GetTicks();

		playersUpdate();
		updateEntities();
		updateMobs();
		addNewEntities();
		addNewMobileEntities();
		removeDeadEntities();

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

void MultiplayerGame::updateEntities() {
	MapData* mapa = view->getMapData();

	map<int,Entity*>::iterator iter;
	for ( iter = entities.begin() ; iter != entities.end() ; ++iter ) {
		(iter->second)->updateDamageTaken();
	}
}

void MultiplayerGame::updateMobs(){
	MapData* mapa = view->getMapData();

	map<int,MobileEntity*>::iterator mobIter;
	for (mobIter = mobileEntities.begin() ; mobIter != mobileEntities.end() ; ++mobIter ) {
		(mobIter->second)->update(mapa);
		(mobIter->second)->updateDamageTaken();
	}

	map<int,ArtificialIntelligence*>::iterator iaIter;
	for ( iaIter = ias.begin() ; iaIter != ias.end() ; ++iaIter ){
		(iaIter->second)->update(mapa);
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
		if ((*player)->hasGolem() && golemsMap.find((*player)->getName()) == golemsMap.end() ) {
			createGolem(*player);
		}
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

	for (map<int,MobileEntity*>::iterator it = mobileEntities.begin() ; it != mobileEntities.end() ; ++it){
		MobileEntity* ent = it->second;
		MobileEntityInfo* info = new MobileEntityInfo();
		info->setId(it->first);
		info->setName(ent->getName());
		info->setEntity(ent);
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

map<int,EntityInfo*> MultiplayerGame::getEntityInfo(){

	map<int,EntityInfo*> infos;

	for (map<int,Entity*>::iterator it = entities.begin() ; it != entities.end() ; ++it){
		Entity* ent = it->second;
		EntityInfo* info = new EntityInfo();
		info->setId(it->first);
		info->setName(ent->getName());
		info->setClassName(ent->getClassName());
		info->setEntity(ent);
		int col = ent->getCoordinates().getCol();
		int row = ent->getCoordinates().getRow();
		Coordinates* tmp = new Coordinates(row,col);
		info->setInitCoordinates(tmp);
		infos[it->first] = info;
	}

	return infos;

}

vector<int> MultiplayerGame::getDeletedEntities(){
	return deletedEntities;
}

void MultiplayerGame::addNewMobileEntities() {
	MapData* mapData = view->getMapData();
	vector<MobileEntity* > newMobiles = mapData->getnewMobileEntities();

	std::vector<MobileEntity *>::const_iterator iter;
	for (iter = newMobiles.begin(); iter != newMobiles.end(); ++iter) {

		MobileEntity* current = *iter;

		Coordinates coords = current->getCoordinates();

		addMobileEntity(current, coords);
	}

	mapData->cleanNewMobileEntities();
}

void MultiplayerGame::addNewEntities() {
	MapData* mapData = view->getMapData();

	vector<Entity* > newEntities = mapData->getNewEntities();

	std::vector<Entity *>::const_iterator iter;
	for (iter = newEntities.begin(); iter != newEntities.end(); ++iter) {
		Entity* current = *iter;

		addEntity(current, current->getCoordinates());
	}

	view->getMapData()->cleanNewEntities();
}

int MultiplayerGame::getEntityId(Entity* entity) {
	map<int,Entity*>::iterator it;
	for (it = entities.begin() ; it != entities.end() ; ++it){
		Entity* current = it->second;

		if (current == entity)
			return it->first;
	}

	std::cout << "Entity no encontrada" << std::endl;
	return -1;
}

void MultiplayerGame::removeDeadEntities() {
	MapData* mapData = view->getMapData();

	list<Entity* > deadEntities = mapData->getDeadEntities();

		std::list<Entity *>::const_iterator iter;
		for (iter = deadEntities.begin(); iter != deadEntities.end(); ++iter) {
			Entity* current = *iter;

			removeEntity( getEntityId(current) );
		}
}

int MultiplayerGame::addMobileEntity(MobileEntity* entity, Coordinates coordiantes){

	entity->setCoordinates(coordiantes.getRow(), coordiantes.getCol());
	int x = Tile::computePosition(coordiantes.getRow(),coordiantes.getCol(),true)->getX();
	int y = Tile::computePosition(coordiantes.getRow(),coordiantes.getCol(),true)->getY();
	entity->setPos(x,y,0);
	int newId = lastAddedView + 1;
	mobileEntities[newId] = entity;
	lastAddedView = newId;

	return newId;
}

void MultiplayerGame::removeMobileEntity(int id){
	if (mobileEntities.count(id) != 0){
		if (ias.count(id) != 0){
			delete ias[id];
			ias.erase(id);
		}
		mobileEntities.erase(id);

		//TODO: SACARLO TAMBIEN DE MAP DATA

		deletedMobileEntities.push_back(id);
	}
}


int MultiplayerGame::addEntity(Entity* entity, Coordinates coordiantes){

	entity->setCoordinates(coordiantes.getRow(), coordiantes.getCol());
	int newId = lastAddedView + 1;
	entities[newId] = entity;
	lastAddedView = newId;

//	view->getMapData()->addItem(coordiantes.getRow(), coordiantes.getCol(),(Item*)entity);


	return newId;
}

void MultiplayerGame::removeEntity(int id){
	if (entities.count(id) != 0){

		entities.erase(id);
		//TODO: SACARLO TAMBIEN DE MAP DATA

		deletedEntities.push_back(id);
	}
}



MultiplayerGame::~MultiplayerGame() {
	// TODO Auto-generated destructor stub
}



