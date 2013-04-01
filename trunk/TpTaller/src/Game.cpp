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
#include <model/ConfigurationReader.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#define BACKGROUND_IMAGE_PATH	"resources/background.png"

#define Default_w	800		// px
#define Default_h	600		// px
#define Default_bpp 0		// px
SDL_Surface* pantalla;
SDL_Rect posFondo;
SDL_Surface* fondo;
Map* map;
Personaje* personaje;
PersonajeVista* personajeVista;

Game::Game(ConfigurationReader* cfgReader) {
	// TODO Auto-generated constructor stub

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

	//Buscamos info sobre la resolucion del escritorio
	const SDL_VideoInfo* escritorio = SDL_GetVideoInfo();

	//Creamos la ventana
	pantalla = SDL_SetVideoMode(Default_w, Default_h, 32, SDL_HWSURFACE);

	//La hacemos fullscreen
	/* int flag = 1;
	 flag = SDL_WM_ToggleFullScreen(pantalla);
	 if (flag == 0) {
	 printf("Unable to go fullscreen: %s\n", SDL_GetError());
	 exit(1);
	 }*/

	//Cargamos la imagen de fondo
	SDL_Surface* imagen = IMG_Load(BACKGROUND_IMAGE_PATH);
	if (imagen == NULL) {
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		exit(1);
	}

	float escalaX = (float) pantalla->w / imagen->w;
	float escalaY = (float) pantalla->h / imagen->h;
	SDL_Surface* fondo_tmp = rotozoomSurfaceXY(imagen, 0, escalaX, escalaY, 0);
	fondo = SDL_DisplayFormatAlpha(fondo_tmp);
	//Liberamos las variables temporales.
	SDL_FreeSurface(imagen);
	SDL_FreeSurface(fondo_tmp);

	posFondo.x = posFondo.y = 0;
	posFondo.w = fondo->w;
	posFondo.h = fondo->h;
}

void draw() {
	//Borramos y redibujamos todo en cada ciclo
	SDL_FillRect(pantalla, NULL, 0);
	SDL_BlitSurface(fondo, NULL, pantalla, &posFondo);

	map->Draw(pantalla);

	Vector2* cam = map->GetCamera();
	personajeVista->UpdateCameraPos(cam->GetX(), cam->GetY());
	personajeVista->Mostrar(pantalla);
	delete cam;
	// Actualiza la pantalla
	SDL_Flip(pantalla);
	SDL_Delay(20);
}

void endDrawing() {
	//Libera la imagen
	SDL_FreeSurface(fondo);

	//Cierra SDL
	SDL_Quit();
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

	endDrawing();

	return EXIT_EVENT;
}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

