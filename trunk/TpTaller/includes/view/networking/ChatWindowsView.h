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
#include <model/entities/player/Player.h>

using namespace std;

class ChatWindowsView {
public:
	ChatWindowsView(SDL_Surface* screen);
	virtual ~ChatWindowsView();
	bool drawChatWindow();
	bool draw_text(string text);
	string get_text(void);
	void setChat(Chat* chat);
	void setPlayer(Player* player);
	Chat * getChat();
	void drawChatView();
	void handle_events(SDL_Event event);
	bool isActive();
private:

	SDL_Surface* chatWindow;

	SDL_Rect clip;
	int pos;
	SDL_Surface* screen;
	TTF_Font* font;
	SDL_Colour text_colour;
	bool state;
	std::string text;
	Chat * chat;
	Player* player;
};

#endif /* CHATWINDOWSVIEW_H_ */

