/*
 * TextHandler.h
 *
 *  Created on: Apr 9, 2013
 *      Author: lucas
 */

#ifndef TEXTHANDLER_H_
#define TEXTHANDLER_H_

#include <SDL/SDL_ttf.h>
#include <string>
#include <map>


using namespace std;

class TextHandler {
public:
	TextHandler();
	SDL_Color getColor(int r, int g, int b);
	void applyTextOnSurface(string text,SDL_Surface* surface,int x, int y, string font, SDL_Color color);
	void applyTextOnSurfaceCentered(string text,SDL_Surface* surface,int x, int y, string font, SDL_Color color);



	virtual ~TextHandler();
private:
	//Map: key: font name; value = font pointer
	map<string,TTF_Font*> fontMap;

};

#endif /* TEXTHANDLER_H_ */
