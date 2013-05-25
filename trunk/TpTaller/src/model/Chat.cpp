/*
 * Chat.cpp
 *
 *  Created on: 12/05/2013
 *      Author: damian
 */

#include <model/Chat.h>

Chat::Chat() {
	this->playerName = "";
	this->receptor = "";
	this->msjRec = "";
	enable = false;
	newline = false;
	change = false;
}

void Chat::Enable()
{
	this->enable = true;
}

void Chat::Disable()
{
	this->enable = false;
}

bool Chat::isEnable()
{
	return this->enable;
}

bool Chat::NewLine()
{
	return this->newline;
}

// Cuando se hace click sobre un personaje se debe llamar a
// esta funcion pasandole el nombre dle tipo
void Chat::setReceptor(string receptor)
{
	if(receptor==this->playerName)
	{
		this->Disable();
	}else{
	this->receptor = receptor;}
}

/*ChatController* Chat::getChatController()
{
	return this->controlador;
}*/

void Chat::assignPlayer(string name)
{
	this->playerName = name;
}

bool Chat::hasChange()
{
	return this->change;
}

void Chat::newMessageSend(ChatMessage* msj)
{
//	this->change=true;
	messagesSend.push_back(msj);
	ChatMessage* aux= new ChatMessage();
	string auxmsj=msj->getMSJ();
	replace(auxmsj.begin(), auxmsj.end(), '/', ' ');
	aux->setMSJ(auxmsj);
	aux->setSender(this->playerName);
	aux->setReceptor(this->playerName);
	this->messagesReceive.push_back(aux);
	this->msjSend = "";
}

void Chat::newMessageReceive(ChatMessage* msj)
{
//	this->change=true;
	string aux = msj->getMSJ();
	replace(aux.begin(), aux.end(), '/', ' ');
	msj->setMSJ(aux);
	messagesReceive.push_back(msj);
	//this->msjSend = "";
	this->receptor = msj->getSender();
}

string Chat::getReceptor()
{
	return this->receptor;
}

/*
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
*/

void Chat::setMessageSend(vector<ChatMessage*> message){
	this->messagesSend = message;
}

void Chat::setMessageReceive(vector<ChatMessage*> message){
	this->messagesReceive = message;
}

vector<ChatMessage*> Chat::getMessagesSend()
{
	return this->messagesSend;
}

vector<ChatMessage*> Chat::getMessagesReceive()
{
	return this->messagesReceive;
}

void Chat::setWrittingMsj(string msj)
{
	this->msjSend = msj;
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
	return this->messagesSend.empty();
}

void Chat::update(ChatUpdate* update)
{
	this->msjRec = update->getSender()+": "+update->getMessage();
	cout << this->msjRec;
}

void Chat::clearNewMessagesSend()
{
	for(unsigned i=0; i < this->messagesSend.size(); i++)
	{
	//	cout<<"se va a eliminar el msj "<<messagesSend[i]->getMSJ()<<endl;
		messagesSend.pop_back();
	}
	//cout<<"verificacion cantidad de datos "<<this->messagesSend.size()<<endl;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Chat& chat){
	out << chat.enable << " " << chat.messagesSend.size() << " ";

	for (unsigned i = 0 ; i < chat.messagesSend.size() ; i++){
		out << *(chat.messagesSend[i]) << " ";
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
		ChatMessage* msj = new ChatMessage();
		in >> *msj;
		chat.messagesSend.push_back(msj);
	}

	return in;
}


Chat::~Chat() {
	// TODO Auto-generated destructor stub
}

