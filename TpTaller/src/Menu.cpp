/*
 * Menu.cpp
 *
 *  Created on: 23/03/2013
 *      Author: tomas
 */

#include <Menu.h>
#include <model/ConfigurationReader.h>
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <iostream>

#define BACK_IMG "resources/lich.jpg"
#define NUM_BUTTONS 2
#define BUTTON_0 "resources/new_game_button.png"
#define BUTTON_1 "resources/exit_button.png"
#define BACK_MUSIC  "resources/sound/pirates.mp3"

namespace std {

int default_w = 1024;
int default_h = 768;
int default_bpp = 0;



SDL_Rect pos[NUM_BUTTONS];

Menu::Menu() {
	//La linea siguiente es para que la window se centre
	putenv("SDL_VIDEO_CENTERED=1");

	SDL_Init(SDL_INIT_EVERYTHING);

	//const SDL_VideoInfo *info = SDL_GetVideoInfo();
	//if (!info)
	screen = SDL_SetVideoMode(default_w, default_h, default_bpp,
			SDL_HWSURFACE);
	/*else
		screen = SDL_SetVideoMode(info->current_w, info->current_h,
				info->vfmt->BytesPerPixel/8, SDL_HWSURFACE);
	 */

	if (!screen) {
		fprintf(stderr, "No se pudo establecer el modo de video: %s",
				SDL_GetError());
		exit(1);
	}
	startMusic();
}

void blitButtons(SDL_Surface* screen){

	// SDL_Surface* buttons[NUM_BUTTONS];

}

void Menu::showMenu() {
	SDL_Surface *background = IMG_Load(BACK_IMG);
	if (!background) {
		fprintf(stderr, "No se ha podido cargar la imagen de fondo: %s\n",
				SDL_GetError());
		exit(1);
	}
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.h = background->h;
	dest.w = background->w;
	SDL_BlitSurface(background, NULL, screen, &dest);

	blitButtons(screen);
	SDL_UpdateRects(screen, 1, &dest);
	SDL_FreeSurface(background);
}

/* Lucas: esto hecho de esta forma demora 4 segundos, directamente
	ciclando sobre el event en main tarda 1 sec, para revisar */
MenuEvent Menu::getEvent() {
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type==SDL_QUIT)
		return EXIT_EVENT;
	return NOTHING_EVENT;
}

void Menu::runConfigMenu(){

}
void Menu::startMusic(){
	// Inicializamos la librería SDL_Mixer

	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,\
			MIX_DEFAULT_CHANNELS, 4096) < 0) {

		cerr << "Subsistema de Audio no disponible" << endl;
		exit(1);
	}

	// Cargamos un sonido

	musica = Mix_LoadMUS(BACK_MUSIC);

	if(!musica) {
		cerr << "No se puede cargar el sonido" << endl;
		exit(1);

	}


	Mix_PlayMusic(musica,-1);

}

Menu::~Menu() {
	// TODO Auto-generated destructor stub
	SDL_FreeSurface(screen);
	Mix_FreeMusic(musica);
	SDL_Quit();

}

} /* namespace std */
