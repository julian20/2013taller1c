/*
 * ChatController.h
 *
 *  Created on: 13/05/2013
 *      Author: damian
 */

#ifndef CHATCONTROLLER_H_
#define CHATCONTROLLER_H_
#include <SDL/SDL.h>
#include <string>
#include <model/Chat.h>
#include <controller/PlayerController.h>
#include <iostream>
#include <algorithm>

using namespace std;

class ChatController {
public:
	ChatController(PlayerController* playerController);

	void handle_events(SDL_Event ev);
	virtual ~ChatController();
	bool isReady();
	void notMoreReady();
	string getText();
	void resetString();
	void setChat(Chat* chat);
private:
	PlayerController* playerController;
	bool ready;
	string text;
	string receptor;
	Chat* chat;
};

#endif /* CHATCONTROLLER_H_ */
