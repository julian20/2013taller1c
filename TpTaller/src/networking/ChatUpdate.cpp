/*
 * ChatUpdate.cpp
 *
 *  Created on: 14/05/2013
 *      Author: damian
 */

#include <networking/ChatUpdate.h>
#include <sstream>
ChatUpdate::ChatUpdate() {
	receiver="";
	sender="";
	msj="";
}
void ChatUpdate::setReceiver(string r)
{
	receiver=r;
}
void ChatUpdate::setSender(string r)
{
	sender=r;
}
void ChatUpdate::setMessage(string r)
{
	msj=r;
}
string ChatUpdate::getReceiver()
{
	return this->receiver;
}
string ChatUpdate::getSender()
{
	return this->sender;
}
string ChatUpdate::getMessage()
{
	return this->msj;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const ChatUpdate& update){

	out << update.sender << " " << update.msj << " " << update.receiver  << " ";
	return out;
}

	//Operator to load an object from a stream
istream& operator >>(std::istream& in, ChatUpdate& update){

	string sender;
	in >> sender;
	update.setSender(sender);


	string msj;
	in >> msj;
	update.setMessage(msj);

	string receiver;
	in >> receiver;
	update.setReceiver(receiver);


	return in;
}
ChatUpdate::~ChatUpdate() {
	// TODO Auto-generated destructor stub
}

