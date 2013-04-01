/*
 * MenuView.cpp
 *
 *  Created on: 25/03/2013
 *      Author: tomas
 */

#include <view/MenuView.h>

#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include <iostream>

#define BACK_IMG "resources/lich.jpg"
#define BACK_MUSIC  "resources/sound/pirates.ogg"
#define START_LAUGH "resources/sound/laugh.wav"
#define START_TAUNT "resources/sound/darkness.wav"
#define ENVIRONMENT_WINDOW "SDL_VIDEO_CENTERED=1"

namespace std {

int default_w = 1024;
int default_h = 768;
int default_bpp = 0;

MenuView::MenuView() {

	musica = NULL;
	sonido = NULL;
	darknessVoice = NULL;

	//La linea siguiente es para que la window se centre
	char environment_setting[] = ENVIRONMENT_WINDOW;
	putenv(environment_setting);

	SDL_Init(SDL_INIT_EVERYTHING);

	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	if (!info) {
		screen = SDL_SetVideoMode(default_w, default_h, default_bpp,
				SDL_HWSURFACE | SDL_RESIZABLE);
	} else {
		screen = SDL_SetVideoMode(info->current_w, info->current_h,
				info->vfmt->BytesPerPixel / 8, SDL_HWSURFACE | SDL_RESIZABLE);

	}

	if (!screen) {
		cerr << "No se pudo establecer el modo de video: " << SDL_GetError()
				<< endl;
		exit(1);
	}

}

SDL_Surface* scaleImage(SDL_Surface* screen, SDL_Surface* image) {

	float scaleX = (float) screen->w / image->w;
	float scaleY = (float) screen->h / image->h;
	SDL_Surface* image_tmp = rotozoomSurfaceXY(image, 0, scaleX, scaleY, 0);
	SDL_Surface* scaleImage = SDL_DisplayFormatAlpha(image_tmp);

	SDL_FreeSurface(image_tmp);
	return scaleImage;

}

void MenuView::initScreen() {
	SDL_Surface *background_image = IMG_Load(BACK_IMG);
	if (!background_image) {
		cerr << "No se ha podido cargar la imagen de fondo: " << SDL_GetError()
				<< endl;
		exit(1);
	}

	SDL_Surface* background = scaleImage(screen, background_image);
	SDL_FreeSurface(background_image);

	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.h = background->h;
	dest.w = background->w;
	SDL_BlitSurface(background, NULL, screen, &dest);

	SDL_UpdateRects(screen, 1, &dest);

	SDL_FreeSurface(background);
}

void MenuView::initButtons(int numButtons, const char** buttons_released,
		const char** buttons_pressed, const MenuEvent* buttons_events) {

	buttons.resize(numButtons);

	for (int i = 0; i < numButtons; i++) {
		SDL_Surface* released = IMG_Load(buttons_released[i]);
		SDL_Surface* pressed = IMG_Load(buttons_pressed[i]);

		SDL_Rect pos;
		pos.x = screen->w / 2 - released->w / 2;
		pos.y = screen->h / 3 - released->h / 2 + i * released->h * 2;
		pos.h = released->h;
		pos.w = released->w;

		MenuEvent event = buttons_events[i];

		buttons[i] = new Button(pressed, released, pos, event);
		buttons[i]->draw(screen);
	}
}

void MenuView::initMusic() {
	// Inicializamos la librería SDL_Mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
			MIX_DEFAULT_CHANNELS, 4096) < 0) {
		cerr << "Subsistema de Audio no disponible" << SDL_GetError() << endl;
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

void MenuView::startLaugh() {
	// Cargamos un sonido

	sonido = Mix_LoadWAV(START_LAUGH);

	if (sonido == NULL) {
		cerr << "No se puede cargar el sonido de la risa" << SDL_GetError()
				<< endl;
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

void MenuView::startVoice() {
	// Cargamos un sonido
	darknessVoice = Mix_LoadWAV(START_TAUNT);
	if (darknessVoice == NULL) {
		cerr << "No se puede cargar el darknessVoice de darkness"
				<< SDL_GetError() << endl;
		exit(1);
	}

	// Establecemos el volumen para el darknessVoice
	int volumen = 1000;
	Mix_VolumeChunk(darknessVoice, volumen);

	// Introducimos el sonido en el canal
	// En el canal 1 con una reproducción (1)
	Mix_PlayChannel(0, darknessVoice, 0);
}

MenuView::~MenuView() {
	SDL_FreeSurface(screen);
	Mix_FreeMusic(musica);
	Mix_FreeChunk(sonido);
	Mix_FreeChunk(darknessVoice);
	Mix_CloseAudio();
	SDL_Quit();
}

} /* namespace std */
