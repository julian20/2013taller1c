/*
 * ChatWindowsView.h
 *
 *  Created on: 01/05/2013
 *      Author: damian
 */

#ifndef CHATWINDOWSVIEW_H_
#define CHATWINDOWSVIEW_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

class ChatWindowsView {
public:
	ChatWindowsView(SDL_Surface*, int, int, int, int, bool);
	virtual ~ChatWindowsView();
	bool draw(void);
	bool draw_text(void);
	string get_text(void);

	void handle_events(SDL_Event event);
private:
	SDL_Rect clip;
	SDL_Surface* _screen;
	TTF_Font* font;
	SDL_Colour text_colour;
	bool focused;
	std::string text;
	bool password;
};

#endif /* CHATWINDOWSVIEW_H_ */

