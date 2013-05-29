/*
 * ChatMessage.cpp
 *
 *  Created on: 15/05/2013
 *      Author: damian
 */

#include <networking/ChatMessage.h>

ChatMessage::ChatMessage() {
	this->msj = "";
	this->receptor = "";
	this->sender = "";
}

string ChatMessage::getMSJ(){
	return msj;
}

void ChatMessage::setMSJ(string msj){
	this->msj = msj;
}

string ChatMessage::getReceptor(){
	return receptor;
}

void ChatMessage::setReceptor(string receptor){
	this->receptor = receptor;
}

string ChatMessage::getSender(){
	return sender;
}

void ChatMessage::setSender(string sender){
	this->sender = sender;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const ChatMessage& msj){
	out << msj.receptor << " " << msj.msj << " " << msj.sender ;

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, ChatMessage& chatmsj){
	string sender, receptor, msj;
	in >> receptor;
	in >> msj;
	in >> sender;

	chatmsj.setReceptor(receptor);
	chatmsj.setMSJ(msj);
	chatmsj.setSender(sender);

	return in;

}

ChatMessage::~ChatMessage() {
}

