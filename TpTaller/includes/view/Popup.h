/*
 * Popup.h
 *
 *  Created on: May 19, 2013
 *      Author: mint
 */

#ifndef POPUP_H_
#define POPUP_H_

#include <Events.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
using namespace std;

class Popup{
public:
	static void popupWindow(string message);
private:
	Popup();
};

#endif /* POPUP_H_ */
