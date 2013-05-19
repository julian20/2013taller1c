/*
 * Chat.h
 *
 *  Created on: 12/05/2013
 *      Author: damian
 */

#ifndef CHAT_H_
#define CHAT_H_
#include <string>
#include <networking/ChatUpdate.h>
#include <networking/ChatMessage.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iostream>
#include <algorithm>
//#include <controller/ChatController.h>
#include <map>

using namespace std;

class Chat {
public:
	Chat();
	void newMessageSend( ChatMessage* msj);
	void newMessageReceive( ChatMessage* msj);

	void assignPlayer(string name);
	virtual ~Chat();
	void setWrittingMsj(string msj);
	//ChatController* getChatController();

	vector<ChatMessage*> getMessagesSend();
	vector<ChatMessage*> getMessagesReceive();

	void setMessageSend(vector<ChatMessage*> message);
	void setMessageReceive(vector<ChatMessage*> message);


	void clearNewMessagesSend();
	void setMessage(vector<ChatMessage*> message);
	string getMessageSend();
	string getMessagerec();

	void setReceptor(string receptor);
	string getReceptor();
	bool NewLine();
	bool isEmpty();
	bool hasChange();
	void update(ChatUpdate* update);
	void Enable();
	void Disable();
	bool isEnable();

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const Chat&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, Chat&);

private:
	bool change;
	bool enable;
	string playerName;
	string receptor;
	string msjRec;
	string msjSend;
	bool newline;

	//ChatController* controlador;
	//primero reciebe el nombre a quien manda el string y luego el msj
	vector<ChatMessage*> messagesSend;
	vector<ChatMessage*> messagesReceive;

};

#endif /* CHAT_H_ */
