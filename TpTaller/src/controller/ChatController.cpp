/*
 * ChatController.cpp
 *
 *  Created on: 13/05/2013
 *      Author: damian
 */

#include <controller/ChatController.h>

ChatController::ChatController(PlayerController* playerController) {
	this->chat=NULL;
	this->ready=false;
	this->text="";
	this->playerController=playerController;
}

void ChatController::setChat(Chat* chat)
{
	this->chat=chat;
}
void ChatController::resetString()
{
	this->text="";
}
void ChatController::handle_events(SDL_Event ev) {

	if ((ev.type == SDL_MOUSEBUTTONDOWN)
			&& (ev.button.button == SDL_BUTTON_LEFT)) {
			int x = ev.button.x;
			int y = ev.button.y;
			if(this->playerController->clickAnotherPlayer(x,y))
				{
					this->chat->Enable();
					this->chat->setReceptor(this->playerController->getLastPlayerTouch());
				//	this->chat->Disable();
				}
	} else if (ev.type == SDL_KEYDOWN ) {
		if (ev.key.keysym.sym == SDLK_KP_ENTER || ev.key.keysym.sym == SDLK_RETURN)
				{
					this->chat->newMessage(this->text);
					this->text="";
				}
		else{
		char c = (char) ev.key.keysym.sym;
		if (c >= 'a' && c <= 'z') {
			SDLMod m = ev.key.keysym.mod;
			c = (m & KMOD_CAPS) || (m & KMOD_SHIFT) ?
					toupper((char) ev.key.keysym.sym) :
					(char) ev.key.keysym.sym;
			this->text += c;
		} else if ((c <= '1' || c >= '0')
				&& ev.key.keysym.sym != SDLK_LSHIFT && ev.key.keysym.sym !=SDLK_RSHIFT) {
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
	}
}
	this->chat->setWrittingMsj(this->text);
}
string ChatController::getText()
{
	return this->text;
}
void ChatController::notMoreReady()
{
	this->ready=false;
}
bool ChatController::isReady()
{
	return this->ready;
}
ChatController::~ChatController() {
	// TODO Auto-generated destructor stub
}

