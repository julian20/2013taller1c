/*
 * Game.cpp
 *
 *  Created on: 22/03/2013
 */
#include <stdio.h>

#include <Game.h>
#include <view/Map.h>
#include <view/PersonajeVista.h>
#include <model/Personaje.h>
#include <model/persistence/ConfigurationReader.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_mixer.h>

#define DEFAULT_W	800		// px
#define DEFAULT_H	600		// px
#define DEFAULT_BPP 0		// px
#define FRAMES_PER_SECOND	30
#define GAME_MUSIC "resources/sound/gameMusic.mp3"

SDL_Surface* pantalla;
SDL_Rect posFondo;
SDL_Surface* fondo;
Map* map;
Personaje* personaje;
PersonajeVista* personajeVista;

Game::Game(ConfigurationReader* cfgReader) {

	MapData* mapData = new MapData(50, 30);

	mapData->SetTileType(MapData::SOIL, 5, 5);
	mapData->SetTileType(MapData::WATER, 7, 5);

	personaje = new Personaje();

	map = new Map(mapData);
	map->AssignPersonaje(personaje);
	map->SetUpPersonajes();

	personajeVista = new PersonajeVista(personaje, "resources/foo4.png");
	mapData->AddPersonaje(0, 0, personaje);
	initMusic();
}

void getEvent() {
}

void refreshMap() {
}

void refreshEntities() {
}

void refreshCharacters() {
}

void startDrawing() {
	SDL_Init(SDL_INIT_EVERYTHING);

	//Buscamos info sobre la resolucion del escritorio y creamos la pantalla
	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	if (!info) {
		pantalla = SDL_SetVideoMode(DEFAULT_W, DEFAULT_H, DEFAULT_BPP,
				SDL_HWSURFACE | SDL_RESIZABLE);
	} else {
		pantalla = SDL_SetVideoMode(info->current_w, info->current_h,
				info->vfmt->BytesPerPixel / 8, SDL_HWSURFACE | SDL_RESIZABLE);

	}
	//La hacemos fullscreen
	/*int flag = 1;
	 flag = SDL_WM_ToggleFullScreen(pantalla);
	 if (flag == 0) {
	 printf("Unable to go fullscreen: %s\n", SDL_GetError());
	 exit(1);
	 }
	 */

}

void draw() {
	//Borramos y redibujamos en cada ciclo
	SDL_FillRect(pantalla, NULL, 0);
	map->Draw(pantalla);

	Vector2* cam = map->GetCamera();
	personajeVista->UpdateCameraPos(cam->GetX(), cam->GetY());
	personajeVista->Mostrar(pantalla);
	delete cam;
	// Actualiza la pantalla
	SDL_Flip(pantalla);
	SDL_Delay(20);
}

MenuEvent Game::run() {

	startDrawing();

	SDL_Event event;
	bool exit = false;
	while (!exit) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == 1) {
				Vector2* camera = map->GetCamera();
				map->ClickOn(	event.button.x, event.button.y,
								event.button.button);
			}

			if (event.type == SDL_QUIT)
				exit = true;
			//personajeVista->Mostrar();
		}

		//Actualizo la parte visual, sin mostrarla todavia
		//1. Mapa
		map->Update();
		// 2. Entidades estaticas
		//       refreshEntities();
		// 3. Personaje/s
		personaje->Update();

		// En un futuro, aca se comprueban las colisiones.
		// y se corrige la posicion del personaje.

		// Dibujo
		draw();
		SDL_Delay(1000 / FRAMES_PER_SECOND);
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
	//startLaugh();

	// Cargamos la musica
	musica = Mix_LoadMUS(GAME_MUSIC);

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

