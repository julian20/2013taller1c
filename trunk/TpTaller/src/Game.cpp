/*
 * Game.cpp
 *
 *  Created on: 22/03/2013
 */
//STL
#include <stdio.h>
#include <sstream>

//SDL
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

//Ours
#include <Game.h>
#include <view/entities/PlayerView.h>
#include <view/sound/SoundEffectHandler.h>
#include <model/entities/player/Player.h>
#include <model/persistence/PersistentConfiguration.h>
#include <model/Logs/Logs.h>

#define ICON "resources/icon.png"
#define POINTER "resources/pointer.png"
#define POINTER_CLICKED "resources/pointerClicked.png"
#define CLICK_ANIMATION "resources/click.png"

Game::Game(PersistentConfiguration* configuration, bool multiplayer) :
		clickAnimation(CLICK_ANIMATION, 22) {
	this->multiplayer = multiplayer;

	mapData = configuration->getMapData();

	this->gameConfig = configuration->getAnimationConfiguration();
	this->textHandler = new TextHandler();
	this->fps = configuration->getAnimationConfiguration()->getFps();
	this->fpsUpdatingTimer = 0;
	this->tempFps = 0;
	Tile::setTileWidth(gameConfig->getTileWidth());
	Tile::setTileHeight(gameConfig->getTileHeight());

	initScreen();

	initMusic();
	SoundEffectHandler::initialize();
	EntityViewMap* viewMap = configuration->getEntityViewMap();
	this->mapView = new MapView(mapData, screen, viewMap);
	this->mapView->setTextureHolder(configuration->getTextureHolder());
	this->cameraController = new MapCameraController(this->mapView->getCamera(),
			gameConfig->getMovementMargin());

	setUpCharacters(mapView, mapData, viewMap);
	setUpEntities(mapView, mapData);

	PlayerController* playerController = new PlayerController();
	playerController->setMapCameraView(mapView->getCamera());
	playerController->setMapData(mapData);
	playerController->setPlayer(personaje);

	this->mapController = new MapController(mapView, mapData, playerController);
	this->mapController->generateEventList(multiplayer);
	openAudio = false;

	personajeVista = (configuration->getViewList())[0];
//	iaMap= new map<string,ArtificialIntelligence*>;

	if (multiplayer) {
		chatController = new ChatController(playerController);
		chatView = new ChatWindowsView(screen);
	}

	SDL_Surface* pointerTmp = IMG_Load(POINTER);
	pointer = SDL_DisplayFormatAlpha(pointerTmp);
	SDL_FreeSurface(pointerTmp);

	SDL_Surface* pointerClickedTmp = IMG_Load(POINTER_CLICKED);
	pointerClicked = SDL_DisplayFormatAlpha(pointerClickedTmp);
	SDL_FreeSurface(pointerClickedTmp);

}
Chat* Game::getChat() {
	return this->chat;
}
void Game::setChat(Chat* chat) {
	this->chat = chat;
	chatController->setChat(chat);
	chatView->setChat(chat);
}
void Game::setUpEntities(MapView* map, MapData* mapData) {

}

void getEvent() {
}

void refreshMap() {
}

void Game::setUpCharacters(MapView* map, MapData* mapData,
		EntityViewMap* viewMap) {

	for (int i = 0; i < viewMap->getNCols(); i++) {

		for (int j = 0; j < viewMap->getNRows(); j++) {

			list<EntityView*> aList = viewMap->getListAtRowAndCol(j, i);
			if (!aList.empty()) {
				list<EntityView*>::iterator it;
				for (it = aList.begin(); it != aList.end(); ++it) {
					EntityView* view = *it;
					if (!view)
						continue;
					Entity* entity = view->getEntity();
					if (entity->getClassName() == "Player") {
						personaje = (Player*) entity;

						Coordinates coord = personaje->getCoordinates();
						mapData->addPlayer(coord.getRow(), coord.getCol(),
								personaje);
					} else if (entity->getClassName() == "MobileEntity") {

						MobileEntity* entityToAdd = (MobileEntity*) entity;

						ArtificialIntelligence* ia =
								new ArtificialIntelligence();
					//	ia->setEntity(entityToAdd);
						string name = entity->getName();
						ias.push_back(ia);
						Coordinates coord = entityToAdd->getCoordinates();
						mapData->addMobileEntity(coord.getRow(), coord.getCol(),
								entityToAdd);

					}
				}
			}
		}
	}

	map->setUpMobileEntities();
	map->assignMobileEntity(personaje);

}

void refreshEntities() {
}

void refreshCharacters() {
}

void Game::initScreen() {
	//Buscamos info sobre la resolucion del escritorio y creamos la screen
	bool configured = false;

	//La linea siguiente es para que la window se centre
	char environment_setting[] = "SDL_VIDEO_CENTERED=1";
	putenv(environment_setting);
	if (gameConfig->screenAutoConfig()) {
		const SDL_VideoInfo *info = SDL_GetVideoInfo();
		screen = SDL_SetVideoMode(info->current_w, info->current_h,
				info->vfmt->BytesPerPixel / 8, SDL_HWSURFACE);
		configured = true;
	}

	if (!configured) {
		screen = SDL_SetVideoMode(this->gameConfig->getDefaultScreenWidth(),
				this->gameConfig->getDefaultScreenHeight(),
				this->gameConfig->getDefaultBPP(), SDL_HWSURFACE);
	}

	if (!screen) {
		Logs::logErrorMessage(
				"Unable to get video mode: " + string(SDL_GetError()));
		exit(1);
	}
	SDL_WM_SetCaption("Purge - Rise of the brotherhood", "Purge");
	SDL_Surface* icon = IMG_Load(ICON);
	if (icon != NULL) {
		SDL_WM_SetIcon(icon, NULL);
	}
	if (gameConfig->fullscreen()) {
		//La hacemos fullscreen
		int flag = 1;
		flag = SDL_WM_ToggleFullScreen(screen);
		if (flag == 0) {
			Logs::logErrorMessage(
					"Unable to go fullscreen: " + string(SDL_GetError()));
		}
	}
	SDL_ShowCursor(SDL_DISABLE);
}

string intToString(int number) {
	stringstream ss; //create a stringstream
	ss << number; //add number to the stream
	return ss.str(); //return a string with the contents of the stream
}

void Game::setAnimationStartup() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	clickedLocation.x = x;
	clickedLocation.y = y;
	clickedLocation.h = pointer->h;
	clickedLocation.w = pointer->w;
	clickAnimation.repeatNTimes(1);
}

SDL_Rect Game::updateMousePosition() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.h = pointer->h;
	rect.w = pointer->w;
	return rect;
}

void Game::draw() {
	//Borramos y redibujamos en cada ciclo
	SDL_FillRect(screen, NULL, 0);
	Position* cam = mapView->getCamera()->getPosition();
	mapView->draw(cam);
	if (multiplayer)
		chatView->drawChatView();
	delete cam;

	fpsUpdatingTimer++;

	if (fpsUpdatingTimer == 10) {
		tempFps = fps;
		fpsUpdatingTimer = 0;
	}
	// Actualiza la screen
	textHandler->applyTextOnSurface("FPS: " + intToString(tempFps), screen, 800,
			40, "baramond", textHandler->getColor(255, 0, 0));
	SDL_Rect rect = updateMousePosition();
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1)) {
		SDL_BlitSurface(pointerClicked, NULL, screen, &rect);
		setAnimationStartup();
	} else {
		SDL_BlitSurface(pointer, NULL, screen, &rect);
		clickAnimation.blitNextFrame(screen, clickedLocation);
	}

	SDL_Flip(screen);
}

MenuEvent Game::run() {

	initScreen();
	SDL_Event event;
	active = true;

	while (active) {
		int ticks = SDL_GetTicks();
		cameraController->cameraMoveListener();
		while (SDL_PollEvent(&event)) {

			mapController->clickListener(event);
			if (multiplayer)
				chatController->handle_events(event);
			if (event.type == SDL_QUIT)
				active = false;

		}

		playersUpdate();

		draw();

		applyFPS(ticks);
	}

	return EXIT_EVENT;

}

void Game::applyFPS(int timer) {
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

void Game::initMusic() {
	// Inicializamos la librería SDL_Mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		Logs::logErrorMessage(
				"Subsistema de audio no disponible: " + string(SDL_GetError()));
		openAudio = false;
		musica = NULL;
		return;
	}

	// Cargamos la musica
	musica = Mix_LoadMUS(this->gameConfig->getGameMusicSrc().c_str());

	if (!musica) {
		Logs::logErrorMessage(
				"No se puede cargar el sonido: " + string(SDL_GetError()));
		musica = NULL;
		return;
	}
	Mix_VolumeMusic(40);
	Mix_FadeInMusic(musica, -1, 3000);

}

void Game::playersUpdate() {

	personaje->update(mapData);
	personaje->setHasChanged(false);

	for (list<Player*>::iterator player = otherPlayers.begin();
			player != otherPlayers.end(); ++player) {
		(*player)->update(mapData);
		(*player)->setHasChanged(false);
	}
/*		for (list<ArtificialIntelligence*>::iterator autoPlayer = ias.begin();
	 autoPlayer != ias.end(); ++autoPlayer) {
	 (*autoPlayer)->update(mapData);
	 //(*autoPlayer)->setHasChanged(false);
	 }*/

}

void Game::addNewPlayer(Player* player, PlayerView* view, Coordinates* coords) {
	player->setCoordinates(coords->getRow(), coords->getCol());
	MapData* map = mapView->getMapData();
	map->addPlayer(coords->getRow(), coords->getCol(), player);
	Coordinates newCoords = player->getCoordinates();
	mapView->addNewPlayerView(view, newCoords);

	otherPlayers.push_back(player);
	otherPlayerViews.push_back(view);

}

void Game::addNewMobileEntity(MobileEntity* entity, MobileEntityView* view, Coordinates* coords, int id){
	entity->setCoordinates(coords->getRow(), coords->getCol());
	MapData* map = mapView->getMapData();
	map->addMobileEntity(coords->getRow(), coords->getCol(),entity);
	Coordinates newCoords = entity->getCoordinates();
	mapView->addNewMobileEntityView(view, newCoords);

	mobileEntities[id] = entity;
	mobileEntitiesView[id] = view;

}

void Game::removeMobileEntity(int id){
	MobileEntityView* view = mobileEntitiesView[id];
	mapView->removeMobileEntity(view);
	// TODO: Estas lineas estan comentadas porque al borrarlo dejo a otro thread accediendo a algo que esta en estado invalido.
//	delete mobileEntities[id];
//	delete mobileEntitiesView[id];
}

void Game::handleMobileEntityUpdates(vector<MobileEntityUpdate*> mobileEntityUpdates) {
	for (size_t i = 0; i < mobileEntityUpdates.size(); i++) {
		int id = mobileEntityUpdates[i]->getId();
		if (mobileEntities.count(id) == 0)
			continue;

		mobileEntities[id]->updateFromServer(mobileEntityUpdates[i]);

	}
}

PlayerView* Game::getPlayerView() {
	return personajeVista;
}

list<PlayerEvent*> Game::getEvents() {
	return mapController->getEvents();
}

void Game::cleanEvents() {
	mapController->cleanEvents();
}

MapCameraView* Game::getMapCameraView() {
	return mapView->getCamera();
}
MapData* Game::getMapData() {
	return mapView->getMapData();
}

bool Game::isActive() {
	return active;
}

void Game::setActive() {
	active = true;
}

void Game::setInactive() {
	active = false;
}

Game::~Game() {
	delete mapView;
	delete mapController;
	delete cameraController;
	delete textHandler;
	if (musica != NULL)
		Mix_FreeMusic(musica);
	SoundEffectHandler::close();
	if (openAudio)
		Mix_CloseAudio();
	SDL_FreeSurface(pointer);
	SDL_FreeSurface(pointerClicked);
	SDL_FreeSurface(screen);
}

