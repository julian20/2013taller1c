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

Game::Game(PersistentConfiguration* configuration) {

	MapData* mapData = configuration->getMapData();

	this->gameConfig = configuration->getAnimationConfiguration();
	this->textHandler = new TextHandler();
	this->fps = 0;
	this->fpsUpdatingTimer = 0;
	this->tempFps = 0;

	initScreen();
	initMusic();

	EntityViewMap* viewMap = configuration->getEntityViewMap();
	this->mapView = new MapView(mapData, screen, viewMap);
	this->mapView->setTextureHolder(configuration->getTextureHolder());
	this->cameraController = new MapCameraController(
			this->mapView->getCamera());

	setUpCharacters(mapView, mapData, viewMap);
	setUpEntities(mapView, mapData);

	PlayerController* playerController = new PlayerController();
	playerController->setMapCameraView(mapView->getCamera());
	playerController->setMapData(mapData);
	playerController->setPlayer(personaje);

	this->mapController = new MapController(mapView, mapData, playerController);
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
					if (entity->getClassName() == "Personaje") {
						//TODO - Lucas: "personaje" deberia ser una lista de personajes
						personaje = (Player*) entity;

						Vector2* pos = personaje->getCurrentPos();
						float x = pos->GetX();
						float y = pos->GetY();
						cout << x << "  " << y << endl;
						mapData->addPersonaje((int) x, (int) y, personaje);
					}
				}
			}
		}
	}

	map->SetUpPersonajes();

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
		screen = SDL_SetVideoMode(info->current_w, info->current_h,
				info->vfmt->BytesPerPixel / 8, SDL_HWSURFACE);
		configured = true;
	}

	if (!configured) {
		screen = SDL_SetVideoMode(this->gameConfig->getDefaultScreenWidth(),
				this->gameConfig->getDefaultScreenHeight(),
				this->gameConfig->getDefaultBPP(), SDL_HWSURFACE);
	}

	if (gameConfig->fullscreen()) {
		//La hacemos fullscreen
		int flag = 1;
		flag = SDL_WM_ToggleFullScreen(screen);
		if (flag == 0) {
			//TODO escribir en el log
			printf("Unable to go fullscreen: %s\n", SDL_GetError());
			exit(1);
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
	textHandler->applyTextOnSurface("FPS: " + intToString(tempFps), screen, 30,
			40, "baramond", textHandler->getColor(255, 0, 0));
	// Actualiza la screen
	SDL_Flip(screen);
}

MenuEvent Game::run() {

	initScreen();
	SDL_Event event;
	bool exit = false;
	while (!exit) {
		int ticks = SDL_GetTicks();
		cameraController->cameraMoveListener();
		while (SDL_PollEvent(&event)) {

			mapController->clickListener(event);

			if (event.type == SDL_QUIT)
				exit = true;

			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				exit = true;
			//personajeVista->Mostrar();
		}

		//Actualizo la parte visual, sin mostrarla todavia
		//1. Mapa
		mapView->Update();
		// 2. Entidades estaticas
		//       refreshEntities();
		// 3. Personaje/s
		personaje->Update();

		// En un futuro, aca se comprueban las colisiones.
		// y se corrige la posicion del personaje.

		// Dibujo}

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
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
			MIX_DEFAULT_CHANNELS, 4096) < 0) {
		cerr << "Subsistema de Audio no disponible" << SDL_GetError() << endl;
		exit(1);
	}

	// Cargamos la musica
	musica = Mix_LoadMUS(this->gameConfig->getGameMusicSrc().c_str());

	if (!musica) {
		cerr << "No se puede cargar el sonido:" << SDL_GetError() << endl;
		exit(1);
	}
	Mix_VolumeMusic(500);
	Mix_FadeInMusic(musica, -1, 3000);

}

Game::~Game() {
	delete mapView;
	delete mapController;
	delete cameraController;
	delete textHandler;
	Mix_FreeMusic(musica);
	Mix_CloseAudio();
	// TODO Auto-generated destructor stub
}
