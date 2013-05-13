/*
 * Chat.cpp
 *
 *  Created on: 12/05/2013
 *      Author: damian
 */

#include <model/Chat.h>

Chat::Chat() {
	this->playerName="";

}
void Chat::assignPlayer(string name)
{
	this->playerName=name;
}
void Chat::newMessage(string name, string msj)
{
	messages[name]=msj;
}

Chat::~Chat() {
	// TODO Auto-generated destructor stub
}

