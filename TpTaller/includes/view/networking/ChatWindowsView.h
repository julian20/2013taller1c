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
#include <vector>
#include <string>
#include <iostream>
#include <model/Chat.h>

using namespace std;

class ChatWindowsView {
public:
	ChatWindowsView();
	virtual ~ChatWindowsView();
	bool drawChatWindow(SDL_Surface* screen);
	bool draw_text(string text);
	string get_text(void);
	void setChat(Chat* chat);
	Chat * getChat();
	void drawChatView(SDL_Surface* screen);
	void handle_events(SDL_Event event);
	bool isActive();
private:
	SDL_Rect clip;
	int pos;
	SDL_Surface* _screen;
	TTF_Font* font;
	SDL_Colour text_colour;
	bool state;
	std::string text;
	Chat * chat;
};

#endif /* CHATWINDOWSVIEW_H_ */

