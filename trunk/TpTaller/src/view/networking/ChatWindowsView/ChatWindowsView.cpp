/*
 * ChatWindowsView.cpp
 *
 *  Created on: 01/05/2013
 *      Author: damian
 */

#include <view/networking/ChatWindowsView.h>
#include <networking/ChatMessage.h>
#include <view/TextHandler.h>
#define FONT "resources/fonts/robot.ttf"
#define WINDOWSHEIGHT 100

ChatWindowsView::~ChatWindowsView() {
	// TODO Auto-generated destructor stub
}

ChatWindowsView::ChatWindowsView(){

	this->state = true;
	this->chat=NULL;
	this->pos=0;
	this->clip.x = 0;
	this->clip.y = 0;
	this->clip.h = WINDOWSHEIGHT;

	font = TTF_OpenFont(FONT,20);
	SDL_Colour colour = {1, 1, 1 };
	//cout << "asigna color" << endl;
	this->text_colour = colour;
}
bool ChatWindowsView::isActive()
{
	return this->state;
}
void ChatWindowsView::setPlayer(Player* player)
{
	this->player=player;
}
void ChatWindowsView::drawChatView(SDL_Surface* screen)
{
	if(chat->isEnable())
	{
		this->drawChatWindow(screen);
		this->pos = 80;
		vector<ChatMessage*> v=chat->getMessagesReceive();

		this->draw_text(chat->getMessageSend());

		int size = v.size();

		for (int i = size - 1 ; i >= 0 ; i--){
			pos -= 20;
			if (pos < 0) break;
			string s = v[i]->getSender() + ": " + v[i]->getMSJ();
			this->draw_text(s);
 		}

//		string recibido=v[0];
//
//		string enviado=v[1];
//		//this->text=;
//		this->pos=60;
//		if(recibido!="") this->draw_text(recibido);
//		this->pos=80;
//		this->draw_text(enviado);
//		if(this->chat->NewLine())
//		{
//
//		}
	}
}
void ChatWindowsView::setChat(Chat* chat)
{
	this->chat=chat;
}
bool ChatWindowsView::drawChatWindow(SDL_Surface* screen) {
	this->clip.w = screen->w;
	this->_screen=screen;
	return SDL_FillRect(screen, &this->clip, 0x0D7A3E);
}

bool ChatWindowsView::draw_text(string texto) {
	string display;
	display = texto;
	SDL_Surface* msg ;
	msg = TTF_RenderText_Solid(this->font, texto.c_str(), this->text_colour);
	SDL_Rect tmp = this->clip;
	tmp.x += 5;
	tmp.y += pos;
	return SDL_BlitSurface(msg, NULL, this->_screen, &tmp);
}

std::string ChatWindowsView::get_text(void) {
	return this->text;
}

