/*
 * Chat.cpp
 *
 *  Created on: 12/05/2013
 *      Author: damian
 */

#include <model/Chat.h>

Chat::Chat() {
	this->playerName="";
	this->receptor="";
	newline=false;
	this->controlador= new ChatController();
}
bool Chat::NewLine()
{
	return this->newline;
}
//cuando se hace click sobre un personaje se debe llamar a esta funcion pasandole el nombre dle tipo
void Chat::setReceptor(string receptor)
{
	this->receptor=receptor;
}
ChatController* Chat::getChatController()
{
	return this->controlador;
}
void Chat::assignPlayer(string name)
{
	this->playerName=name;
}
void Chat::newMessage(string name, string msj)
{
	messages[name]=msj;
}
string Chat::getMessage()
{
	this->newline=false;
	if(this->controlador->isReady())
	{
		this->newMessage(this->receptor,this->controlador->getText());
		this->controlador->notMoreReady();
		this->controlador->resetString();
		this->newline=true;
	}
	return this->playerName+": "+(this->controlador->getText());

}

Chat::~Chat() {
	// TODO Auto-generated destructor stub
}

