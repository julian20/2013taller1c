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
#define FONT_SIZE 20

#define WINDOW_MARGIN 50

#define WINDOWSHEIGHT 100
#define CHATWINDOW "resources/fonts/chat-window.png"

ChatWindowsView::~ChatWindowsView() {
	// TODO Auto-generated destructor stub
}

ChatWindowsView::ChatWindowsView(SDL_Surface* screen){

	this->state = true;
	this->chat=NULL;
	this->pos=0;
	this->clip.x = WINDOW_MARGIN;
	this->clip.y = screen->h - WINDOWSHEIGHT;
	this->clip.h = WINDOWSHEIGHT;
	this->clip.w = screen->w - 2*WINDOW_MARGIN;
	this->player = NULL;
	this->screen = screen;

	SDL_Surface* img = IMG_Load(CHATWINDOW);
	if (!img){
		Logs::logErrorMessage("ChatWindowView: No se pudo cargar la imagen de fondo de la ventana de chat");
	}

	float scaleX = (float) clip.w / img->w;
	float scaleY = (float) clip.h / img->h;

	chatWindow = rotozoomSurfaceXY(img,0,scaleX,scaleY,0);
	if (!chatWindow){
		Logs::logErrorMessage("ChatWindowView: No se pudo cargar la imagen de fondo de la ventana de chat");
	}



	font = TTF_OpenFont(FONT,FONT_SIZE);
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
void ChatWindowsView::drawChatView()
{
	if(chat->isEnable()){


		this->drawChatWindow();
		this->pos = screen->h - FONT_SIZE;
		vector<ChatMessage*> v=chat->getMessagesReceive();

		this->draw_text(chat->getMessageSend());

		int size = v.size();

		for (int i = size - 1 ; i >= 0 ; i--){
			pos -= FONT_SIZE;
			if (pos < clip.y) break;
			string s = v[i]->getSender() + ": " + v[i]->getMSJ();
			this->draw_text(s);
 		}

		SDL_Flip(chatWindow);
	}
}
void ChatWindowsView::setChat(Chat* chat){
	this->chat=chat;
}

bool ChatWindowsView::drawChatWindow() {

	SDL_BlitSurface(chatWindow,NULL,screen,&this->clip);

	return true;
}

bool ChatWindowsView::draw_text(string texto) {
	string display;
	display = texto;
	SDL_Surface* msg ;
	msg = TTF_RenderText_Solid(this->font, texto.c_str(), this->text_colour);
	SDL_Rect tmp = this->clip;
	tmp.x += 5;
	tmp.y = pos;
	return SDL_BlitSurface(msg, NULL, screen, &tmp);
}

std::string ChatWindowsView::get_text(void) {
	return this->text;
}

