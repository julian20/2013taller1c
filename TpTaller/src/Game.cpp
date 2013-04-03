/*
 * Game.cpp
 *
 *  Created on: 22/03/2013
 */
#include <stdio.h>

#include <Game.h>
#include <view/MapView.h>
#include <view/entities/PersonajeVista.h>
#include <model/entities/personaje/Personaje.h>
#include <model/persistence/PersistentConfiguration.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_mixer.h>

#define DEFAULT_W	800		// px
#define DEFAULT_H	600		// px
#define DEFAULT_BPP 0		// px
#define FRAMES_PER_SECOND	30
#define GAME_MUSIC "resources/sound/gameMusic.ogg"

Game::Game(PersistentConfiguration* configuration) {

	MapData* mapData = new MapData(50, 30);

	mapData->SetTileType("soil", 5, 5);
	mapData->SetTileType("water", 7, 5);
	mapData->SetTileType("tree", 9, 5);

	map = new MapView(mapData);
	map->setTextureHolder(configuration->getTextureHolder());

	setUpCharacters(map, mapData);

	initMusic();
}

void getEvent() {
}

void refreshMap() {
}

void Game::setUpCharacters(MapView* map, MapData* mapData) {
	//TODO: Posiblemente se levante del cfg reader esto
	personaje = new Personaje();
	personajeVista = new PersonajeVista(personaje, "resources/foo5.png");
	mapData->addPersonaje(5, 5, personaje);
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
	SDL_Init(SDL_INIT_EVERYTHING);

	//Buscamos info sobre la resolucion del escritorio y creamos la pantalla
	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	if (!info) {
		pantalla = SDL_SetVideoMode(DEFAULT_W, DEFAULT_H, DEFAULT_BPP,
				SDL_HWSURFACE);
	} else {
		pantalla = SDL_SetVideoMode(info->current_w, info->current_h,
				info->vfmt->BytesPerPixel / 8, SDL_HWSURFACE);

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

void Game::draw() {
	//Borramos y redibujamos en cada ciclo
	SDL_FillRect(pantalla, NULL, 0);
	map->Draw(pantalla);

	Position* cam = map->GetCamera();
	personajeVista->UpdateCameraPos(cam->getX(), cam->getY());
	personajeVista->Mostrar(pantalla);
	delete cam;
	// Actualiza la pantalla
	SDL_Flip(pantalla);
}

MenuEvent Game::run() {

	initScreen();

	SDL_Event event;
	bool exit = false;
	while (!exit) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == 1) {
				Position* camera = map->GetCamera();
				map->ClickOn(event.button.x, event.button.y,
						event.button.button);
			}

			if (event.type == SDL_QUIT)
				exit = true;

			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
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

