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
using namespace std;

class ChatController {
public:
	ChatController();
	void handle_events(SDL_Event ev);
	virtual ~ChatController();
	bool isReady();
	void notMoreReady();
	string getText();
	void resetString();
private:
	bool ready;
	string text;
};

#endif /* CHATCONTROLLER_H_ */
