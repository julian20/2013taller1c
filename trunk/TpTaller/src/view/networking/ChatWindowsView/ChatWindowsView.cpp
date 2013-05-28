/*
 * ChatWindowsView.cpp
 *
 *  Created on: 01/05/2013
 *      Author: damian
 */

#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include <model/Logs/Logs.h>
#include <view/networking/ChatWindowsView.h>
#include <networking/ChatMessage.h>
#include <view/TextHandler.h>
#define FONT "resources/fonts/robot.ttf"
#define WINDOWSHEIGHT 100
#define CHATWINDOW "resources/fonts/chat-window.png"

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
	this->player = NULL;
	this->_screen = NULL;


	font = TTF_OpenFont(FONT,20);
	SDL_Colour colour = {255, 255, 255 };
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
	}
}
void ChatWindowsView::setChat(Chat* chat)
{
	this->chat=chat;
}
bool ChatWindowsView::drawChatWindow(SDL_Surface* screen) {
	this->clip.w = screen->w;
	this->_screen=screen;

//	Uint32 rmask, gmask, bmask, amask;
//	rmask = 0x000000ff;
//	gmask = 0x0000ff00;
//	bmask = 0x00ff0000;
//	amask = 0xff000000;
//
//	SDL_Surface* retval = SDL_CreateRGBSurface(SDL_SWSURFACE, clip.w, clip.h, 32,
//			rmask, gmask, bmask, amask);
//
//	SDL_FillRect(retval, NULL, 0x90000000);
//
//	SDL_BlitSurface(retval,NULL,screen,&clip);
//
//	SDL_UpdateRects(screen,1,&clip);
//
//
//	return true;
	return SDL_FillRect(screen, &this->clip, 0x000000);

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

