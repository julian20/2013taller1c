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
#define BACK_MUSIC  "resources/sound/pirates.ogg"
#define START_LAUGH "resources/sound/laugh.wav"
#define START_TAUNT "resources/sound/darkness.wav"
#define ENVIRONMENT_WINDOW "SDL_VIDEO_CENTERED=1"

namespace std {

int default_w = 1024;
int default_h = 768;
int default_bpp = 0;

typedef struct button{
	SDL_Rect pos;
	SDL_Surface* surface;
	MenuEvent event;
}Button;

Button buttons[NUM_BUTTONS];

const char* buttons_released[NUM_BUTTONS] = {"resources/buttons/new_game_button.png","resources/buttons/exit_button.png"};
const char* buttons_pressed[NUM_BUTTONS] = {"resources/buttons/new_game_pressed.png","resources/buttons/exit_pressed.png"};
const MenuEvent button_events[NUM_BUTTONS] = {NEWGAME_EVENT,EXIT_EVENT};

Menu::Menu() {
	//La linea siguiente es para que la window se centre
	char environment_setting[] = ENVIRONMENT_WINDOW;
	putenv(environment_setting);

	SDL_Init(SDL_INIT_EVERYTHING);

	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	if (!info)
		screen = SDL_SetVideoMode(default_w, default_h, default_bpp,
				SDL_HWSURFACE | SDL_RESIZABLE);
	else{
		screen = SDL_SetVideoMode(info->current_w, info->current_h,
				info->vfmt->BytesPerPixel / 8, SDL_HWSURFACE | SDL_RESIZABLE);

	}
	if (!screen) {
		fprintf(stderr, "No se pudo establecer el modo de video: %s",
				SDL_GetError());
		exit(1);
	}
}

/*
 * <init>
 */

void Menu::startScreen(){
	SDL_Surface *background_image = IMG_Load(BACK_IMG);
	if (!background_image) {
		fprintf(stderr, "No se ha podido cargar la imagen de fondo: %s\n",
				SDL_GetError());
		exit(1);
	}

	float escalaX = (float) screen->w / background_image->w;
	float escalaY = (float) screen->h / background_image->h;
	SDL_Surface* background_tmp = rotozoomSurfaceXY(background_image, 0, escalaX, escalaY, 0);
	SDL_Surface* background = SDL_DisplayFormatAlpha(background_tmp);

	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.h = background->h;
	dest.w = background->w;
	SDL_BlitSurface(background, NULL, screen, &dest);

	SDL_UpdateRects(screen, 1, &dest);

	SDL_FreeSurface(background_image);
	SDL_FreeSurface(background_tmp);
	SDL_FreeSurface(background);
}

void Menu::startButtons(){

	SDL_Rect* rects = (SDL_Rect*) malloc(NUM_BUTTONS*sizeof(SDL_Rect));

	for (int i = 0 ; i < NUM_BUTTONS ; i++){
		buttons[i].surface = IMG_Load(buttons_released[i]);
		buttons[i].pos.x = screen->w/2 - buttons[i].surface->w/2;
		buttons[i].pos.y = screen->h/3 - buttons[i].surface->h/2 + i*buttons[i].surface->h*2;
		buttons[i].pos.h = buttons[i].surface->h;
		buttons[i].pos.w = buttons[i].surface->w;
		buttons[i].event = button_events[i];

		SDL_BlitSurface(buttons[i].surface,NULL,screen,&(buttons[i].pos));
		rects[i] = buttons[i].pos;
	}

	SDL_UpdateRects(screen, NUM_BUTTONS, rects);

	free(rects);

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

void Menu::init(){
	startScreen();
	startButtons();
	startMusic();
}

/*
 * </init>
 */


/*
 * <run>
 */

// Funcion fea
bool mouseIsOnButton(Button button,SDL_Event event){

	return (event.button.x > button.pos.x && event.button.x < button.pos.x + button.pos.w
			&& event.button.y > button.pos.y && event.button.y < button.pos.y + button.pos.h);

}

void Menu::checkPressedButton(SDL_Event event){

	//TODO harcoded for 1 button
	for (int i = 0 ; i < NUM_BUTTONS ; i++){

		if (mouseIsOnButton(buttons[i],event)){
			SDL_FreeSurface(buttons[i].surface);
			buttons[i].surface = IMG_Load(buttons_pressed[i]);
			SDL_BlitSurface(buttons[i].surface, NULL, screen, &(buttons[i].pos));
			SDL_UpdateRects(screen,1,&(buttons[i].pos));
		}

	}

}

MenuEvent Menu::checkReleasedButton(SDL_Event event){

	//TODO harcoded for 1 button
	for (int i = 0 ; i < NUM_BUTTONS ; i++){

		SDL_FreeSurface(buttons[i].surface);
		buttons[i].surface = IMG_Load(buttons_released[i]);
		SDL_BlitSurface(buttons[i].surface, NULL, screen, &(buttons[i].pos));
		SDL_UpdateRects(screen,1,&(buttons[i].pos));

		if (mouseIsOnButton(buttons[i],event)){
			return buttons[i].event;
		}
	}

	return NOTHING_EVENT;

}


MenuEvent Menu::run() {

	SDL_Event event;

	while (SDL_PollEvent(&event)){
		switch (event.type){
		case SDL_QUIT:
			return EXIT_EVENT;
			break;
		case SDL_MOUSEBUTTONDOWN:
			checkPressedButton(event);
			return NOTHING_EVENT;
			break;
		case SDL_MOUSEBUTTONUP:
			return checkReleasedButton(event);
		default:
			return NOTHING_EVENT;
			break;
		}
	}

	return NOTHING_EVENT;
}

/*
 * </run>
 */


void Menu::runConfigMenu() {

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
