/*
 * Game.cpp
 *
 *  Created on: 22/03/2013
 */
#include <stdio.h>

#include <Game.h>
#include <view/entities/PersonajeVista.h>
#include <model/entities/personaje/Personaje.h>
#include <model/persistence/PersistentConfiguration.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_mixer.h>

Game::Game(PersistentConfiguration* configuration) {

	MapData* mapData = configuration->getMapData();

	this->gameConfig = configuration->getAnimationConfiguration();

	initScreen();
	initMusic();

	this->mapView = new MapView(mapData, screen);
	this->mapView->setTextureHolder(configuration->getTextureHolder());
	this->mapController = new MapController(mapView);

	setUpCharacters(mapView, mapData);

}

void getEvent() {
}

void refreshMap() {
}

void Game::setUpCharacters(MapView* map, MapData* mapData) {

	//TODO: Posiblemente se levante del cfg reader esto
	personaje = new Personaje();
	personajeVista = new PersonajeVista(personaje);
	mapData->addPersonaje(0, 0, personaje);
	map->AssignPersonaje(personaje);
	/* Lucas: La linea siguiente antes estaba antes de lo de arriba, pero
	 * yo creo que va despues, como vas a iterar la matriz de personajes
	 * seteandolos si no hiciste addpersonaje?
	 */
	map->SetUpPersonajes();
}

void refreshEntities() {
}

void refreshCharacters() {
}

void Game::initScreen() {

	//Buscamos info sobre la resolucion del escritorio y creamos la screen
	bool configured = false;

	if (gameConfig->screenAutoConfig()){
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


	if (gameConfig->fullscreen()){
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

void Game::draw() {
	//Borramos y redibujamos en cada ciclo

	SDL_FillRect(screen, NULL, 0);
	mapView->Draw();

	Position* cam = mapView->GetCamera();
	personajeVista->UpdateCameraPos(cam->getX(), cam->getY());
	personajeVista->Mostrar(screen);
	delete cam;
	// Actualiza la screen
	SDL_Flip(screen);
}

MenuEvent Game::run() {

	initScreen();

	SDL_Event event;
	bool exit = false;
	while (!exit) {
		mapController->cameraMoveListener();
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

		// Dibujo
		draw();
		unsigned int fps = this->gameConfig->getFps();
		SDL_Delay(1000 / fps);
	}

	return EXIT_EVENT;

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
	Mix_FreeMusic(musica);
	Mix_CloseAudio();
	// TODO Auto-generated destructor stub
}
