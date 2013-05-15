/*
 * ChatUpdate.h
 *
 *  Created on: 14/05/2013
 *      Author: damian
 */

#ifndef CHATUPDATE_H_
#define CHATUPDATE_H_

#include <string>

using namespace std;

class ChatUpdate {
public:
	ChatUpdate();
	void setReceiver(string receiver);
	void setMessage(string message);
	void setSender(string name);

	string getReceiver();
	string getMessage();
	string getSender();

	virtual ~ChatUpdate();



	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const ChatUpdate&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, ChatUpdate&);

private:

		string receiver;
		string sender;
		string msj;

};

#endif /* CHATUPDATE_H_ */
