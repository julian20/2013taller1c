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
	this->msjRec="";
	newline=false;
	change=false;
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
bool Chat::hasChange()
{
	return change;
}
void Chat::newMessage(string name, string msj)
{
	change=true;
	messages[name]=msj;
}
string Chat::getReceptor()
{
	return this->receptor;
}
vector<string> Chat::getMessage()
{
	//primero dibujo los recibidos y abajo los enviados
	vector<string> vector;

	vector.push_back(this->getMessagerec());
	vector.push_back(this->getMessageSend());
	return vector;
}
string Chat::getMessageSend()
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
string Chat::getMessagerec()
{
	return this->msjRec;
}
bool Chat::isEmpty()
{
	return this->messages.empty();
}
void Chat::update(ChatUpdate* update)
{
	this->msjRec=update->getSender()+": "+update->getMessage();
}
Chat::~Chat() {
	// TODO Auto-generated destructor stub
}

