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

const char* buttons_released[NUM_BUTTONS] = { "resources/menu/buttons/SP.png",
		"resources/menu/buttons/MP.png",
		"resources/menu/buttons/Server.png",
		"resources/menu/buttons/Exit.png" };
const char* buttons_pressed[NUM_BUTTONS] = {
		"resources/menu/buttons/SP_pressed.png",
		"resources/menu/buttons/MP_pressed.png",
		"resources/menu/buttons/Server_pressed.png",
		"resources/menu/buttons/Exit_pressed.png" };
const char* buttons_hovered[NUM_BUTTONS] = {
		"resources/menu/buttons/SP_hovered.png",
		"resources/menu/buttons/MP_hovered.png",
		"resources/menu/buttons/Server_hovered.png",
		"resources/menu/buttons/Exit_hovered.png" };
const MenuEvent buttons_events[NUM_BUTTONS] = { NEWGAME_EVENT,
		MULTIPLAYER_GAME_EVENT, SERVER_EVENT, EXIT_EVENT };

Menu::Menu(GameConfiguration* configuration) {

	view = new MenuView(configuration);
	view->initScreen();
	view->initButtons(NUM_BUTTONS, buttons_released, buttons_pressed,
			buttons_hovered, buttons_events);
	view->initMusic();
	closed = false;

}

/*
 * <run>
 */

// Funcion fea
bool mouseIsOnButton(Button* button, SDL_Event event) {

	return (event.button.x > button->pos.x
			&& event.button.x < button->pos.x + button->pos.w
			&& event.button.y > button->pos.y
			&& event.button.y < button->pos.y + button->pos.h);

}

bool mouseHoversOnButton(Button* button) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return (x > button->pos.x && x < button->pos.x + button->pos.w
			&& y > button->pos.y && y < button->pos.y + button->pos.h);

}

void Menu::checkHoveredButton(SDL_Event event) {
	for (int i = 0; i < NUM_BUTTONS; i++) {
		if (mouseHoversOnButton(view->buttons[i])) {
			if (!view->buttons[i]->isPressed()) {
				view->buttons[i]->changeState(BUTTON_HOVERED);
				view->buttons[i]->draw(view->screen);
			}

		} else {
			if (!view->buttons[i]->isPressed()) {
				view->buttons[i]->changeState(BUTTON_RELEASED);
				view->buttons[i]->draw(view->screen);
			} else {
				view->buttons[i]->changeState(BUTTON_PRESSED);
				view->buttons[i]->draw(view->screen);
			}
		}

	}

}
void Menu::checkPressedButton(SDL_Event event) {

	for (int i = 0; i < NUM_BUTTONS; i++) {

		if (mouseIsOnButton(view->buttons[i], event)) {
			if (!view->buttons[i]->isPressed()) {
				view->buttons[i]->changeState(BUTTON_PRESSED);
				view->buttons[i]->draw(view->screen);
			}
		}

	}

}

MenuEvent Menu::checkReleasedButton(SDL_Event event) {

	for (int i = 0; i < NUM_BUTTONS; i++) {

		if (view->buttons[i]->isPressed()) {
			view->buttons[i]->changeState(BUTTON_RELEASED);
			view->buttons[i]->draw(view->screen);
		}

		if (mouseIsOnButton(view->buttons[i], event)) {
			return view->buttons[i]->event;
		}
	}

	return NOTHING_EVENT;

}

MenuEvent Menu::run() {

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
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
			checkHoveredButton(event);
			return NOTHING_EVENT;
			break;
		}
	}

	return NOTHING_EVENT;
}

/*
 * </run>
 */

void Menu::close() {
	view->close();
	closed = true;
}

void Menu::runConfigMenu() {
}

Menu::~Menu() {
	if (!closed)
		close();
	delete view;
}

