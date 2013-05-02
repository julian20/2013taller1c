/*
 * ChatWindowsView.cpp
 *
 *  Created on: 01/05/2013
 *      Author: damian
 */

#include <view/networking/ChatWindowsView.h>
#include <view/TextHandler.h>
#define FONT "resources/fonts/robot.ttf"

ChatWindowsView::~ChatWindowsView() {
	// TODO Auto-generated destructor stub
}

ChatWindowsView::ChatWindowsView(SDL_Surface* screen, int x, int y, int w,
		int h, bool pass) {

	cout << "asigna screen" << endl;
	this->_screen = screen;
	cout << "asigna focused" << endl;
	this->focused = true;
	cout << "asigna pass" << endl;
	this->password = pass;

	cout << "asigna clips" << endl;
	this->clip.x = x;
	this->clip.y = y;
	this->clip.w = w;
	this->clip.h = h;

	font = TTF_OpenFont(FONT,50);
	cout << "crea color" << endl;
	SDL_Colour colour = {1, 1, 1 };
	//cout << "asigna color" << endl;
	this->text_colour = colour;
	cout << "termino de asignar todo" << endl;
}

bool ChatWindowsView::draw(void) {
	return SDL_FillRect(this->_screen, &this->clip, 0x0D7A3E);
}

bool ChatWindowsView::draw_text(void) {
	this->draw();
	string display;
	display = "";
	if (this->password) {
		for (unsigned int i = 0; i < this->text.length(); i++) {
			display += "*";
		}
	} else {
		display = this->text;
	}
	SDL_Surface* msg = NULL;
	msg = TTF_RenderText_Solid(this->font, display.c_str(), this->text_colour);
	SDL_Rect tmp = this->clip;
	tmp.x += 5;
	return SDL_BlitSurface(msg, NULL, this->_screen, &tmp);
}

std::string ChatWindowsView::get_text(void) {
	return this->text;
}

void ChatWindowsView::handle_events(SDL_Event ev) {
	if ((ev.type == SDL_MOUSEBUTTONDOWN)
			&& (ev.button.button == SDL_BUTTON_LEFT)) {
		int x = ev.button.x;
		int y = ev.button.y;

		if (x < this->clip.w + this->clip.x && x > this->clip.x
				&& y < this->clip.h + this->clip.y && y > this->clip.y) {
			this->focused = true;
		} else {
			this->focused = false;
		}
	} else if (ev.type == SDL_KEYDOWN ) {
		char c = (char) ev.key.keysym.sym;
		if (c >= 'a' && c <= 'z') {
			SDLMod m = ev.key.keysym.mod;
			c = (m & KMOD_CAPS) || (m & KMOD_SHIFT) ?
					toupper((char) ev.key.keysym.sym) :
					(char) ev.key.keysym.sym;
			this->text += c;
		} else if ((c <= '1' || c >= '0')
				&& ev.key.keysym.sym != SDLK_LSHIFT) {
			if (ev.key.keysym.mod & KMOD_SHIFT) {
				switch (c) {
				case '1':
					c = '!';
					break;
				case '2':
					c = '"';
					break;
				case '3':
					c = '#';
					break;
				case '4':
					c = '$';
					break;
				case '5':
					c = '%';
					break;
				case '6':
					c = '^';
					break;
				case '7':
					c = '&';
					break;
				case '8':
					c = '*';
					break;
				case '9':
					c = '(';
					break;
				case '0':
					c = ')';
					break;
				case '-':
					c = '_';
					break;
				case '=':
					c = '+';
					break;
				case '[':
					c = '{';
					break;
				case ']':
					c = '}';
					break;
				case ';':
					c = ':';
					break;
				case '\'':
					c = '@';
					break;
				case '#':
					c = '~';
					break;
				case ',':
					c = '<';
					break;
				case '.':
					c = '>';
					break;
				case '/':
					c = '?';
					break;
				default:
					break;
				}
			}
			this->text += c;
			//Si apreta borrar armo un substring con dos letras menos
		}
		if (ev.key.keysym.sym == SDLK_BACKSPACE) {
			int longitud = this->text.length() - 2;
			string aux = (this->text).substr(0, longitud);
			this->text = aux;
		}
		this->draw_text();
	}
}
