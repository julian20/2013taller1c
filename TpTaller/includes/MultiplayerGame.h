/*
 * MultiplayerGame.h
 *
 *  Created on: 11/05/2013
 *      Author: tomas
 */

#ifndef MULTIPLAYERGAME_H_
#define MULTIPLAYERGAME_H_

#include <list>
#include <map>

#include <Game.h>
#include <model/Chat.h>
#include <model/persistence/PersistentConfiguration.h>
#include <model/persistence/ConfigurationReader.h>
#include <model/entities/enemies/Mob.h>
#include <model/entities/Golem/Golem.h>
#include <model/entities/Flag.h>
#include <networking/MobileEntityInfo.h>
#include <networking/EntityInfo.h>
#include <networking/MobUpdate.h>
#include <networking/PlayerEvent.h>
#include <networking/NetworkPlayerController.h>
#include <networking/PlayerUpdate.h>
#include <networking/ChatUpdate.h>
#include <view/entities/MobileEntityView.h>
#include <view/MapView.h>
#include <AI/ArtificialIntelligence.h>
#include <map>
#include <stdlib.h>

using namespace std;

class MultiplayerGame {
public:
	MultiplayerGame(PersistentConfiguration* configuration);
	virtual MenuEvent run();
	virtual void addNewPlayer(Player* player, Coordinates* coordiantes);
	void addEventsToHandle(string playerName, vector<PlayerEvent*> events);
	vector<PlayerUpdate*> getPlayersUpdates();
	vector<MobileEntityUpdate*> getMobileEntitiesUpdates();
	vector<ChatUpdate*> getChatUpdates();
	list<Player*> getPlayers();
	Flag* getFlag();

	// METODOS PARA AGREGAR Y QUITAR MOBILEENTITIES DEL SERVER.
	// TODO LO QUE SE AGREGUE O SE QUITE CON ESTOS METODOS SERA ENVIADO AL SERVER.

	// Agrega una mobile Entity a MultiplayerGame. Devuelve el ID que se le asigno.
	int addMobileEntity(MobileEntity* entity, Coordinates coordiantes);
	// Quita del juego la mobileEntity con el id pasado como parametro.
	void removeMobileEntity(int id);
	// Agrega una Entity a MultiplayerGame. Devuelve el ID que se le asigno.
	int addEntity(Entity* entity, Coordinates coordiantes);
	// Quita del juego la Entity con el id pasado como parametro.
	void removeEntity(int id);


	void createGolemIa(MobileEntity* golem);
	map<int,MobileEntityInfo*> getMobileEntityInfo();
	vector<int> getDeletedMobileEntities();

	map<int, EntityInfo*> getEntityInfo();
	vector<int> getDeletedEntities();

	void deliverMessage(ChatMessage* msj);
	void updateEntities();
	void updateMobs();
	void createFlag(MapData* mapData);
	void createGolem(Player* player);
	virtual ~MultiplayerGame();

private:
	void updatePlayersCoordinates();
	void updateMobsCoordinates();
	void addNewMobileEntities();
	void addNewEntities();
	int getEntityId(Entity* entity);
	void removeDeadEntities();
	void playersUpdate();
	void applyFPS(int timer);

	MapView* view;

	map< string, NetworkPlayerController*> controllers;
	map< string, Golem*> golemsMap;
	list<Player*> players;
	map<int,ArtificialIntelligence*> ias;
	map<MobileEntity*, Coordinates> mobilesCoords;
	map<int,MobileEntity*> mobileEntities;
	vector<int> deletedMobileEntities;
	map<int,Entity*> entities;
	vector<int> deletedEntities;
	int lastAddedView;
	Flag* flag;

	int fps;
	int tempFps;
	int fpsUpdatingTimer;
	GameConfiguration* gameConfig;
};

#endif /* MULTIPLAYERGAME_H_ */
