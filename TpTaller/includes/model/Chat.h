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
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <controller/ChatController.h>
#include <map>

using namespace std;

class Chat {
public:
	Chat();
	void newMessage( string msj);
	void assignPlayer(string name);
	virtual ~Chat();
	void setWrittingMsj(string msj);
	//ChatController* getChatController();
	vector<string> getMessage();
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
	map<string,string> messages;

};

#endif /* CHAT_H_ */
