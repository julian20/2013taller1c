/*
 * Menu.cpp
 *
 *  Created on: 23/03/2013
 *      Author: tomas
 */

#include <Menu.h>
#include <model/ConfigurationReader.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define BACK_IMG "../../resources/Lord_Of_The_Rings_Wallpaper_by_beyondwonderwall.png"
#define NUM_BUTTONS 2
#define BUTTON_0 = "../../resources/new_game_button.png"
#define BUTTON_1 = "../../resources/exit_button.png"

namespace std {

int default_w = 1024;
int default_h = 768;
int default_bpp = 0;



SDL_Rect pos[NUM_BUTTONS];

Menu::Menu() {
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

}

void blitButtons(SDL_Surface* screen){

	SDL_Surface* buttons[NUM_BUTTONS];

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

MenuEvent Menu::getEvent() {
	return EXIT_EVENT;
}

void Menu::runConfigMenu(){

}

Menu::~Menu() {
// TODO Auto-generated destructor stub
	SDL_FreeSurface(screen);
	SDL_Quit();

}

} /* namespace std */
