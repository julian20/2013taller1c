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
#include <model/entities/player/Player.h>
#include <model/persistence/PersistentConfiguration.h>
#include <model/Logs/Logs.h>


Game::Game(PersistentConfiguration* configuration, bool multiplayer) {

	MapData* mapData = configuration->getMapData();

	this->gameConfig = configuration->getAnimationConfiguration();
	this->textHandler = new TextHandler();
	this->fps = 0;
	this->fpsUpdatingTimer = 0;
	this->tempFps = 0;
	Tile::setTileWidth( gameConfig->getTileWidth() );
	Tile::setTileHeight( gameConfig->getTileHeight() );

	initScreen();
	initMusic();

	EntityViewMap* viewMap = configuration->getEntityViewMap();
	this->mapView = new MapView(mapData, screen, viewMap);
	this->mapView->setTextureHolder(configuration->getTextureHolder());
	this->cameraController = new MapCameraController(this->mapView->getCamera(), gameConfig->getMovementMargin());

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

	//Chat* chat= new Chat();

	chatController = new ChatController(playerController);
	chatView = new ChatWindowsView();
	//chatController->setChat(chat);
//	chatView->setChat(chat);
	
}
Chat* Game::getChat()
{
	return this->chat;
}
void Game::setChat(Chat* chat)
{
	this->chat=chat;
	chatController->setChat(chat);
	chatView->setChat(chat);
}
void Game::setUpEntities(MapView* map, MapData* mapData) {

}

void getEvent() {
}

void refreshMap() {
}

void Game::setUpCharacters(MapView* map, MapData* mapData, EntityViewMap* viewMap) {

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
					if (entity->getClassName() == "Personaje") {
						//TODO - Lucas: "personaje" deberia ser una lista de personajes
						personaje = (Player*) entity;

						Coordinates coord = personaje->getCoordinates();
						mapData->addPersonaje(coord.getRow(), coord.getCol(), personaje);
					}
				}
			}
		}
	}

	map->setUpPersonajes();
	map->assignPersonaje(personaje);

}

void refreshEntities() {
}

void refreshCharacters() {
}

void Game::initScreen() {
	//Buscamos info sobre la resolucion del escritorio y creamos la screen
	bool configured = false;

	if (gameConfig->screenAutoConfig()) {
		const SDL_VideoInfo *info = SDL_GetVideoInfo();
		screen = SDL_SetVideoMode(info->current_w, info->current_h, info->vfmt->BytesPerPixel / 8, SDL_HWSURFACE);
		configured = true;
	}

	if (!configured) {
		screen = SDL_SetVideoMode(this->gameConfig->getDefaultScreenWidth(), this->gameConfig->getDefaultScreenHeight(), this->gameConfig->getDefaultBPP(), SDL_HWSURFACE);
	}

	if (!screen){
		Logs::logErrorMessage("Unable to get video mode: " + string(SDL_GetError()));
		exit(1);
	}

	if (gameConfig->fullscreen()) {
		//La hacemos fullscreen
		int flag = 1;
		flag = SDL_WM_ToggleFullScreen(screen);
		if (flag == 0) {
			//TODO escribir en el log
			Logs::logErrorMessage("Unable to go fullscreen: " + string(SDL_GetError()));
		}
	}

}

string intToString(int number) {
	stringstream ss; //create a stringstream
	ss << number; //add number to the stream
	return ss.str(); //return a string with the contents of the stream
}

void Game::draw() {
	//Borramos y redibujamos en cada ciclo
	SDL_FillRect(screen, NULL, 0);
	Position* cam = mapView->getCamera()->getPosition();
	mapView->draw(cam);
	delete cam;

	fpsUpdatingTimer++;

	if (fpsUpdatingTimer == 10) {
		tempFps = fps;
		fpsUpdatingTimer = 0;
	}
		// Actualiza la screen
	chatView->drawChatView(screen);
	textHandler->applyTextOnSurface("FPS: " + intToString(tempFps), screen, 30, 40, "baramond", textHandler->getColor(255, 0, 0));
	SDL_Flip(screen);
}

MenuEvent Game::run() {

	initScreen();
	SDL_Event event;
	active = true;

	while (active) {
		int ticks = SDL_GetTicks();
		//cameraController->cameraMoveListener();
		while (SDL_PollEvent(&event)) {

			mapController->clickListener(event);
			chatController->handle_events(event);
			if (event.type == SDL_QUIT)
				active = false;

			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
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
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 6, 4096) < 0) {
		Logs::logErrorMessage("Subsistema de audio no disponible: " + string(SDL_GetError()));
		openAudio = false;
		musica = NULL;
		return;
	}

	// Cargamos la musica
	musica = Mix_LoadMUS(this->gameConfig->getGameMusicSrc().c_str());

	if (!musica) {
		Logs::logErrorMessage("No se puede cargar el sonido: " + string(SDL_GetError()));
		musica = NULL;
		return;
	}
	Mix_VolumeMusic(500);
	Mix_FadeInMusic(musica, -1, 3000);

}

void Game::playersUpdate(){

	personaje->update();
	personaje->setChange(false);

	for ( list<Player*>::iterator player = otherPlayers.begin() ; player != otherPlayers.end() ; ++player ){
		(*player)->update();
		(*player)->setChange(false);
	}

}

void Game::addNewPlayer(Player* player, PlayerView* view, Coordinates* coords){
	player->setCoordinates(coords->getRow(),coords->getCol());
	mapView->addNewPlayerView(view,*coords);
	MapData* map = mapView->getMapData();
	map->addPersonaje(coords->getRow(),coords->getCol(),player);

	otherPlayers.push_back(player);
	otherPlayerViews.push_back(view);

}

PlayerView* Game::getPlayerView(){
	return personajeVista;
}

list<PlayerEvent*> Game::getEvents(){
	return mapController->getEvents();
}

void Game::cleanEvents(){
	mapController->cleanEvents();
}

MapCameraView* Game::getMapCameraView(){
	return mapView->getCamera();
}
MapData* Game::getMapData(){
	return mapView->getMapData();
}

bool Game::isActive(){
	return active;
}

void Game::setActive(){
	active = true;
}

void Game::setInactive(){
	active = false;
}

/*
Chat* Game::getChat(){
	return chat;
}
*/
Game::~Game() {
	delete mapView;
	delete mapController;
	delete cameraController;
	delete textHandler;
	if (musica != NULL) Mix_FreeMusic(musica);
	if (openAudio) Mix_CloseAudio();
	SDL_FreeSurface(screen);
	// TODO Auto-generated destructor stub
}

