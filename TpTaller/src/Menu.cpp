/*
 * Menu.cpp
 *
 *  Created on: 23/03/2013
 *      Author: tomas
 */
#include <stdlib.h>
#include <iostream>
#include <Menu.h>
#include <view/Button.h>

#include <SDL/SDL_events.h>


namespace std {

const char* buttons_released[NUM_BUTTONS] = {"resources/buttons/new_game_button.png","resources/buttons/exit_button.png"};
const char* buttons_pressed[NUM_BUTTONS] = {"resources/buttons/new_game_pressed.png","resources/buttons/exit_pressed.png"};
const MenuEvent buttons_events[NUM_BUTTONS] = {NEWGAME_EVENT,EXIT_EVENT};

Menu::Menu() {

	view = new MenuView();
	view->initScreen();
	view->initButtons(NUM_BUTTONS,buttons_released,buttons_pressed,buttons_events);
	//view->initMusic();
}

/*
 * <run>
 */

// Funcion fea
bool mouseIsOnButton(Button* button,SDL_Event event){

	return (event.button.x > button->pos.x && event.button.x < button->pos.x + button->pos.w
			&& event.button.y > button->pos.y && event.button.y < button->pos.y + button->pos.h);

}

void Menu::checkPressedButton(SDL_Event event){

	for (int i = 0 ; i < NUM_BUTTONS ; i++){

		if (mouseIsOnButton(view->buttons[i],event)){
			if (!view->buttons[i]->isPressed()){
				view->buttons[i]->changeState();
				view->buttons[i]->draw(view->screen);
			}
		}

	}

}

MenuEvent Menu::checkReleasedButton(SDL_Event event){

	for (int i = 0 ; i < NUM_BUTTONS ; i++){

		if (view->buttons[i]->isPressed()){
			view->buttons[i]->changeState();
			view->buttons[i]->draw(view->screen);
		}


		if (mouseIsOnButton(view->buttons[i],event)){
			return view->buttons[i]->event;
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
	delete view;
}

} /* namespace std */
