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
	enable=false;
	newline=false;
	change=false;
}
void Chat::Enable()
{
	this->enable=true;
}
void Chat::Disable()
{
	this->enable=false;
}
bool Chat::isEnable()
{
	return this->enable;
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
/*ChatController* Chat::getChatController()
{
	return this->controlador;
}*/
void Chat::assignPlayer(string name)
{
	this->playerName=name;
}
bool Chat::hasChange()
{
	return this->change;
}
void Chat::newMessage(ChatMessage* msj)
{
	this->change=true;
	messages.push_back(msj);
	this->msjSend="";


}
string Chat::getReceptor()
{
	return this->receptor;
}
vector<ChatMessage*> Chat::getMessage()
{
//	//primero dibujo los recibidos y abajo los enviados
//	vector<string> vector;
//
//	vector.push_back(this->getMessagerec());
//	vector.push_back(this->getMessageSend());
//	return vector;
	return messages;
}

void Chat::setMessage(vector<ChatMessage*> message){
	this->messages = message;
}
void Chat::setWrittingMsj(string msj)
{
	this->msjSend=msj;
}
//void Chat::setEnable(bool enable)
//{
//	this->enable=enable;
//}
string Chat::getMessageSend()
{
	return this->playerName+": "+this->msjSend;
}
	/*this->newline=false;
	if(this->controlador->isReady())
	{
		this->newMessage(this->receptor,this->controlador->getText());
		this->controlador->notMoreReady();
		this->controlador->resetString();
		this->newline=true;
	}
	return this->playerName+": "+(this->controlador->getText());

}*/
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
	cout<<this->msjRec;
}


//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Chat& chat){
	out << chat.enable << " " << chat.messages.size() << " ";

	for (int i = 0 ; i < chat.messages.size() ; i++){
		out << *(chat.messages[i]) << " ";
	}

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Chat& chat){
	bool enable;
	in >> enable;

	chat.enable;

	int size;
	in >> size;


	for (int i = 0 ; i < size ; i++){
		ChatMessage* msj = new ChatMessage("","","");
		in >> *msj;
		chat.messages.push_back(msj);
	}

	return in;
}


Chat::~Chat() {
	// TODO Auto-generated destructor stub
}

