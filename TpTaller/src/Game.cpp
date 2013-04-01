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


#define DEFAULT_W	800		// px
#define DEFAULT_H	600		// px
#define DEFAULT_BPP 0		// px

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
	map->SetUpPersonajes();

	personajeVista = new PersonajeVista(personaje, "resources/foo4.png");
	mapData->AddPersonaje(0, 0, personaje);

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
				personaje->MoveTo(event.button.x - camera->GetX(),
						event.button.y - camera->GetY());
				map->ClickOn(event.button.x, event.button.y,
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
	}


	return EXIT_EVENT;
}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

