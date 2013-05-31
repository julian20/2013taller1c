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

#include <model/Logs/Logs.h>

#define START_LAUGH "resources/menu/sound/laugh.wav"
#define START_TAUNT "resources/menu/sound/darkness.wav"
#define LOGO "resources/menu/logo.png"
#define ENVIRONMENT_WINDOW "SDL_VIDEO_CENTERED=1"
#define AUDIO_CHANNELS 20
#define DEFAULT_BACKIMG "resources/menu/wastelands.jpg"
#define ICON "resources/icon.png"

using namespace std;
MenuView::MenuView(GameConfiguration* configuration) {

	musica = NULL;
	darknessVoice = NULL;
	screen = NULL;
	gameConfig = configuration;

	//La linea siguiente es para que la window se centre
	char environment_setting[] = ENVIRONMENT_WINDOW;
	putenv(environment_setting);

	SDL_Init(SDL_INIT_EVERYTHING);

	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	if (!info) {
		screen = SDL_SetVideoMode(this->gameConfig->getDefaultScreenWidth(),
				this->gameConfig->getDefaultScreenHeight(),
				this->gameConfig->getDefaultBPP(),
				SDL_HWSURFACE | SDL_RESIZABLE);
	} else {
		screen = SDL_SetVideoMode(1000, 600, info->vfmt->BytesPerPixel / 8,
				SDL_HWSURFACE | SDL_RESIZABLE);

	}
	SDL_WM_SetCaption( "Purge - Rise of the brotherhood", "Purge" );
	SDL_Surface* icon = IMG_Load(ICON);
	if (icon!=NULL){
		SDL_WM_SetIcon(icon, NULL);
	}
	if (!screen) {
		Logs::logErrorMessage(
				"No se pudo establecer el modo de video: "
						+ string(SDL_GetError()));
		exit(1);
	}
	audioOpen = false;
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
	SDL_Surface *background_image = IMG_Load(
			this->gameConfig->getMenuBackImageSrc().c_str());
	if (!background_image) {
		Logs::logErrorMessage(
				"Unable to load background image: " + string(SDL_GetError()));
		background_image = IMG_Load(DEFAULT_BACKIMG);
	}

	SDL_Surface* background = scaleImage(screen, background_image);
	SDL_FreeSurface(background_image);

	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.h = background->h;
	dest.w = background->w;
	SDL_BlitSurface(background, NULL, screen, &dest);
	initImages();
	SDL_UpdateRects(screen, 1, &dest);

	SDL_FreeSurface(background);
}

void MenuView::initButtons(int numButtons, const char** buttons_released,
		const char** buttons_pressed, const char** buttons_hovered, const MenuEvent* buttons_events) {

	buttons.resize(numButtons);

	for (int i = 0; i < numButtons; i++) {
		//TODO chequeo de las imagenes
		SDL_Surface* released = IMG_Load(buttons_released[i]);
		SDL_Surface* pressed = IMG_Load(buttons_pressed[i]);
		SDL_Surface* hovered = IMG_Load(buttons_hovered[i]);

		SDL_Rect pos;
		pos.x = screen->w / 2 - released->w / 2;
		pos.y = screen->h / 3 - released->h / 2 + i * released->h *1.5 + 30;
		pos.h = released->h;
		pos.w = released->w;

		MenuEvent event = buttons_events[i];

		buttons[i] = new Button(pressed, released,hovered, pos, event);
		buttons[i]->draw(screen);
	}
}

void MenuView::initImages() {
	//TODO chequeo de las imagenes
	SDL_Surface* logoTemp = IMG_Load(LOGO);

	float scaleX = (float) screen->w / logoTemp->w /2;
	float scaleY = (float) screen->h / logoTemp->h /5;
	SDL_Surface* logo = rotozoomSurfaceXY(logoTemp, 0, scaleX, scaleY, 0);
	SDL_FreeSurface(logoTemp);

	SDL_Rect pos;
	pos.x = screen->w / 2 - logo->w / 2;
	pos.y = screen->h / 3 - logo->h*1.5;
	pos.h = logo->h;
	pos.w = logo->w;

	SDL_BlitSurface(logo, NULL, screen, &pos);
	SDL_FreeSurface(logo);
}

void MenuView::initMusic() {
	// Inicializamos la librería SDL_Mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 700) < 0) {
		Logs::logErrorMessage(
				"Subsistema de audio no disponible: " + string(SDL_GetError()));
		musica = NULL;
		audioOpen = true;
		return;
	}
	// Cargamos la musica
	musica = Mix_LoadMUS(this->gameConfig->getMenuBackMusicSrc().c_str());

	if (!musica) {
		Logs::logErrorMessage(
				"No se puede cargar el sonido: " + string(SDL_GetError()));
		musica = NULL;
		return;
	}
	Mix_VolumeMusic(30);
	Mix_FadeInMusic(musica, -1, 3000);

}

void MenuView::close() {
	for (unsigned i = 0; i < buttons.size(); i++)
		delete buttons[i];
	if (screen != NULL)
		SDL_FreeSurface(screen);
	if (musica != NULL)
		Mix_FreeMusic(musica);
	Mix_CloseAudio();
}

MenuView::~MenuView() {
	SDL_Quit();
}

