/*
 * Menu.cpp
 *
 *  Created on: 23/03/2013
 *      Author: tomas
 */
#include <stdlib.h>
#include <Menu.h>
#include <model/ConfigurationReader.h>
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_mixer.h>
#include <iostream>

#define BACK_IMG "resources/lich.jpg"
#define BUTTON_0 "resources/buttons/new_game_button.png"
#define BUTTON_0_PRESSED "resources/buttons/new_game_pressed.png"
#define BACK_MUSIC  "resources/sound/pirates.ogg"
#define START_LAUGH "resources/sound/laugh.wav"
#define START_TAUNT "resources/sound/darkness.wav"
#define ENVIRONMENT_WINDOW "SDL_VIDEO_CENTERED=1"

namespace std {

int default_w = 1024;
int default_h = 768;
int default_bpp = 0;

Button buttons[NUM_BUTTONS];

Menu::Menu() {
	//La linea siguiente es para que la window se centre
	char environment_setting[] = ENVIRONMENT_WINDOW;
	putenv(environment_setting);

	SDL_Init(SDL_INIT_EVERYTHING);

	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	if (!info)
		screen = SDL_SetVideoMode(default_w, default_h, default_bpp,
				SDL_HWSURFACE | SDL_RESIZABLE);
	else
		screen = SDL_SetVideoMode(info->current_w, info->current_h,
				info->vfmt->BytesPerPixel / 8, SDL_HWSURFACE | SDL_RESIZABLE);

	if (!screen) {
		fprintf(stderr, "No se pudo establecer el modo de video: %s",
				SDL_GetError());
		exit(1);
	}

	//startMusic();
}

void blitButtons(SDL_Surface* screen) {

	buttons[0].surface = IMG_Load(BUTTON_0);
	if (!buttons[0].surface) {
		fprintf(stderr, "No se ha podido cargar la imagen de boton: %s\n",
				SDL_GetError());
		exit(1);
	}

	SDL_Event event;

	while (SDL_PollEvent(&event)){
		if (event.type == SDL_MOUSEBUTTONDOWN){
			if (event.button.x > buttons[0].pos.x && event.button.x < buttons[0].pos.x + buttons[0].pos.w
					&& event.button.y > buttons[0].pos.y && event.button.y < buttons[0].pos.y + buttons[0].pos.h){
				buttons[0].surface = IMG_Load(BUTTON_0_PRESSED);
			}

		}
	}




	buttons[0].pos.x = screen->w/2 - buttons[0].surface->w/2;
	buttons[0].pos.y = screen->h/3 - buttons[0].surface->h/2;
	buttons[0].pos.h = buttons[0].surface->h;
	buttons[0].pos.w = buttons[0].surface->w;


	SDL_BlitSurface(buttons[0].surface, NULL, screen, &(buttons[0].pos));

}

void Menu::showMenu() {
	SDL_Surface *background_tmp = IMG_Load(BACK_IMG);
	if (!background_tmp) {
		fprintf(stderr, "No se ha podido cargar la imagen de fondo: %s\n",
				SDL_GetError());
		exit(1);
	}

	float escalaX = (float) screen->w / background_tmp->w;
	float escalaY = (float) screen->h / background_tmp->h;
	background_tmp = rotozoomSurfaceXY(background_tmp, 0, escalaX, escalaY, 0);
	SDL_Surface* background = SDL_DisplayFormatAlpha(background_tmp);

	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.h = background->h;
	dest.w = background->w;
	SDL_BlitSurface(background, NULL, screen, &dest);

	blitButtons(screen);
	SDL_UpdateRects(screen, 1, &dest);
	SDL_FreeSurface(background_tmp);
}

/* Lucas: esto hecho de esta forma demora 4 segundos, directamente
 ciclando sobre el event en main tarda 1 sec, para revisar */
MenuEvent Menu::getEvent() {
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
		return EXIT_EVENT;
	return NOTHING_EVENT;
}

void Menu::runConfigMenu() {

}
void Menu::startMusic() {
	// Inicializamos la librería SDL_Mixer

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
			MIX_DEFAULT_CHANNELS, 4096) < 0) {

		cerr << "Subsistema de Audio no disponible" << endl;
		exit(1);
	}
	//startLaugh();

	// Cargamos la musica
	musica = Mix_LoadMUS(BACK_MUSIC);

	if (!musica) {
		cerr << "No se puede cargar el sonido:" << SDL_GetError() << endl;
		exit(1);

	}

	Mix_FadeInMusic(musica, -1, 3000);
	startVoice();

}

void Menu::startLaugh() {
	// Cargamos un sonido

	sonido = Mix_LoadWAV(START_LAUGH);

	if (sonido == NULL) {

		cerr << "No se puede cargar el sonido de la risa" << endl;
		exit(1);

	}

	// Establecemos el volumen para el sonido

	int volumen = 1000;

	Mix_VolumeChunk(sonido, volumen);

	// Creamos un canal

	Mix_AllocateChannels(2);

	// Introducimos el sonido en el canal
	// En el canal 1 con una reproducción (0)

	Mix_PlayChannel(1, sonido, 0);
}

void Menu::startVoice() {
	// Cargamos un sonido

	darknessVoice = Mix_LoadWAV(START_TAUNT);

	if (darknessVoice == NULL) {

		cerr << "No se puede cargar el darknessVoice de darkness" << endl;
		exit(1);

	}

	// Establecemos el volumen para el darknessVoice

	int volumen = 1000;

	Mix_VolumeChunk(darknessVoice, volumen);

	// Introducimos el sonido en el canal
	// En el canal 1 con una reproducción (1)

	Mix_PlayChannel(0, darknessVoice, 0);
}

Menu::~Menu() {
	// TODO Auto-generated destructor stub
	SDL_FreeSurface(screen);
	Mix_FreeMusic(musica);
	Mix_FreeChunk(sonido);
	Mix_FreeChunk(darknessVoice);
	Mix_CloseAudio();
	SDL_Quit();

}

} /* namespace std */
