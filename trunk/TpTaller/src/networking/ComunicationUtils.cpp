/*
 * ComunicationUtils.cpp
 *
 *  Created on: 01/05/2013
 *      Author: tomas
 */

#include <networking/ComunicationUtils.h>

#include <sys/socket.h>

#include <sstream>

#define ITER_LIMIT 100

namespace std {

#define NUMBER_SIZE 10
#define EXTRA 3

ComunicationUtils::ComunicationUtils() {
	// TODO Auto-generated constructor stub

}

void ComunicationUtils::sendNumber(int sockID,int number){

	stringstream nstream;
	int size = NUMBER_SIZE;

	nstream << number;

	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,nstream.str().c_str(),size,MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}

}

int ComunicationUtils::recvNumber(int sockID){

	char nbuffer[NUMBER_SIZE];
	int n = -1;

	int recvSize = 0;
	int i = 0;

	while (recvSize != NUMBER_SIZE){
		recvSize = recv(sockID,nbuffer,NUMBER_SIZE, MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;

	}

	stringstream nstream;
	nstream << nbuffer;
	nstream >> n;

	return n;

}

void ComunicationUtils::sendString(int sockID,string string){

	stringstream sstream;
	sstream << string;

	int size = string.size() + EXTRA;

	ComunicationUtils::sendNumber(sockID,size);

	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,sstream.str().c_str(),size,MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}

}

string ComunicationUtils::recvString(int sockID){

	int size = ComunicationUtils::recvNumber(sockID);

	char sbuffer[size];

	int recvSize = 0;
	int i = 0;

	while (recvSize != size){
		recvSize = recv(sockID,sbuffer,size,MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}

	string srecv = string(sbuffer);

	return srecv;

}

void ComunicationUtils::sendPlayerInfo(int sockID,PlayerInfo* info){

	stringstream infostream;
	infostream << *(info);

	int size = infostream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

	// SEND PLAYER INFO
	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,infostream.str().c_str(),size, MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}
}

PlayerInfo* ComunicationUtils::recvPlayerInfo(int sockID){

	int size = ComunicationUtils::recvNumber(sockID);
	PlayerInfo* info = new PlayerInfo();

	char infobuffer[size];

	int recvSize = 0;
	int i = 0;

	while (recvSize != size){
		recvSize = recv(sockID,infobuffer,size,MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}

	stringstream infostream;
	infostream << infobuffer;

	infostream >> *info;

	return info;

}

void ComunicationUtils::sendPlayerEvent(int sockID,PlayerEvent* event){
	stringstream eventstream;
	eventstream << *(event);

	int size = eventstream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,eventstream.str().c_str(),size, MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}
}

PlayerEvent* ComunicationUtils::recvPlayerEvent(int sockID){

	int size = ComunicationUtils::recvNumber(sockID);
	PlayerEvent* event = new PlayerEvent();
	char eventbuffer[size];

	int recvSize = 0;
	int i = 0;

	while (recvSize != size){
		recvSize = recv(sockID,eventbuffer,size,MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}

	stringstream eventstream;
	eventstream << eventbuffer;
	eventstream >> *event;

	return event;

}

void ComunicationUtils::sendChatUpdate(int sockID,ChatUpdate* update){

	stringstream updatestream;
	updatestream << *update;

	int size = updatestream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

	send(sockID,updatestream.str().c_str(), size, MSG_EOR);

}

void ComunicationUtils::sendPlayerUpdate(int sockID,PlayerUpdate* update){

	stringstream updatestream;
	updatestream << *update;

	int size = updatestream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,updatestream.str().c_str(), size, MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}
}


PlayerUpdate* ComunicationUtils::recvPlayerUpdate(int sockID){

	int size = ComunicationUtils::recvNumber(sockID);
	char updatebuffer[size];
	stringstream updatestream;

	int recvSize = 0;
	int i = 0;

	while (recvSize != size){
		recvSize = recv(sockID,updatebuffer,size,MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}

	updatestream << updatebuffer;

	PlayerUpdate* update = new PlayerUpdate();

	updatestream >> *update;

	return update;

}

ChatUpdate* ComunicationUtils::recvChatUpdate(int sockID){

	int size = ComunicationUtils::recvNumber(sockID);
	char updatebuffer[size];
	stringstream updatestream;

	recv(sockID,updatebuffer,size,MSG_EOR);

	updatestream << updatebuffer;

	ChatUpdate* update = new ChatUpdate();

	updatestream >> *update;

	return update;

}

ComunicationUtils::~ComunicationUtils() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
