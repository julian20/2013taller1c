/*
 * TextHandler.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: lucas
 */

#include <view/TextHandler.h>
#include <SDL/SDL_ttf.h>

using namespace std;

TextHandler::TextHandler() {
	// TODO Auto-generated constructor stub
	TTF_Init();
	fontMap["lazy"]=TTF_OpenFont( "resources/fonts/lazy.ttf", 28 );
	fontMap["baramond"]=TTF_OpenFont( "resources/fonts/Baramond.ttf", 28 );

}



SDL_Color TextHandler::getColor(int r, int g, int b){
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
}

void TextHandler::applyTextOnSurface(string text,SDL_Surface* surface,int x, int y, string font, SDL_Color color){

	SDL_Surface* textSurface;
	TTF_Font* selectedFont;
	//Si la fuente elegida no existe usamos el default
	if ( fontMap.find(font) == fontMap.end())
		selectedFont = fontMap["lazy"];
	else
		selectedFont = fontMap[font];

	textSurface = TTF_RenderText_Solid(selectedFont, text.c_str(), color);
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = textSurface->w;
	rect.h = textSurface->h;
	//Apply the time surface
	SDL_BlitSurface(textSurface, NULL, surface, &rect);

	//Free the time surface
	SDL_FreeSurface(textSurface);
}


void TextHandler::applyTextOnSurfaceCentered(string text,SDL_Surface* surface,int x, int y, string font, SDL_Color color){

	SDL_Surface* textSurface;
	TTF_Font* selectedFont;
	//Si la fuente elegida no existe usamos el default
	if ( fontMap.find(font) == fontMap.end())
		selectedFont = fontMap["lazy"];
	else
		selectedFont = fontMap[font];

	textSurface = TTF_RenderText_Solid(selectedFont, text.c_str(), color);
	SDL_Rect rect;

	rect.x = x-textSurface->w/2;
	rect.y = y;
	rect.w = textSurface->w;
	rect.h = textSurface->h;
	//Apply the time surface
	SDL_BlitSurface(textSurface, NULL, surface, &rect);

	//Free the time surface
	SDL_FreeSurface(textSurface);
}


TextHandler::~TextHandler() {
	map<string, TTF_Font*>::iterator it;
	for(it = fontMap.begin(); it != fontMap.end(); it++) {
		TTF_CloseFont( it->second );
	}

	TTF_Quit();
	// TODO Auto-generated destructor stub
}

