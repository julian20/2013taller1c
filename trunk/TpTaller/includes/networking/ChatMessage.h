/*
 * ChatMessage.h
 *
 *  Created on: 15/05/2013
 *      Author: damian
 */

#ifndef CHATMESSAGE_H_
#define CHATMESSAGE_H_

#include <string>
#include <sstream>

using namespace std;

class ChatMessage {
public:
	ChatMessage(string msj, string receptor, string sender);
	string getMSJ();
	void setMSJ(string msj);
	string getReceptor();
	void setReceptor(string receptor);
	string getSender();
	void setSender(string sender);
	virtual ~ChatMessage();

	friend ostream& operator <<(std::ostream& out, const ChatMessage& msj);
	friend istream& operator >>(std::istream& in, ChatMessage& chatmsj);

private:
	string msj, receptor, sender;
};

#endif /* CHATMESSAGE_H_ */
