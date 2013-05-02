/*
 * ComunicationUtils.cpp
 *
 *  Created on: 01/05/2013
 *      Author: tomas
 */

#include <networking/ComunicationUtils.h>

#include <sys/socket.h>

#include <sstream>


namespace std {

#define NUMBER_SIZE 15*sizeof(char)
#define STRING_SIZE 150*sizeof(char)
#define INFO_SIZE 2*(sizeof(PlayerInfo) + sizeof(Player))
#define EVENT_SIZE 2*(sizeof(PlayerEvent) + sizeof(Vector2))
#define EXTRA 3

ComunicationUtils::ComunicationUtils() {
	// TODO Auto-generated constructor stub

}

void ComunicationUtils::sendNumber(int sockID,int number){

	stringstream nstream;
	int size = NUMBER_SIZE;

	nstream << number;

	send(sockID,nstream.str().c_str(),size,MSG_EOR);

}

int ComunicationUtils::recvNumber(int sockID){

	char nbuffer[NUMBER_SIZE];
	int n = -1;

	recv(sockID,nbuffer,NUMBER_SIZE, MSG_EOR);

	stringstream nstream;
	nstream << nbuffer;
	nstream >> n;

	return n;

}

void ComunicationUtils::sendString(int sockID,string string){

	int size = STRING_SIZE;
	stringstream sstream;
	sstream << string;

	send(sockID,sstream.str().c_str(),size,MSG_EOR);

}

string ComunicationUtils::recvString(int sockID){

	int size = STRING_SIZE;

	char sbuffer[size];

	recv(sockID,sbuffer,size,MSG_EOR);

	string srecv = string(sbuffer);

	return srecv;

}

void ComunicationUtils::sendPlayerInfo(int sockID,PlayerInfo* info){

	stringstream infostream;
	infostream << *(info);

	int size = INFO_SIZE;

	// SEND PLAYER INFO
	send(sockID,infostream.str().c_str(),size, MSG_EOR);

}

PlayerInfo* ComunicationUtils::recvPlayerInfo(int sockID){

	int size = INFO_SIZE;
	PlayerInfo* info = new PlayerInfo();

	char infobuffer[size];

	recv(sockID,infobuffer,size,MSG_EOR);
	stringstream infostream;
	infostream << infobuffer;

	infostream >> *info;

	return info;

}

void ComunicationUtils::sendPlayerEvent(int sockID,PlayerEvent* event){
	stringstream eventstream;
	eventstream << *(event);

	int size = EVENT_SIZE;

	send(sockID,eventstream.str().c_str(),size, MSG_EOR);

}

PlayerEvent* ComunicationUtils::recvPlayerEvent(int sockID){

	int size = EVENT_SIZE;
	PlayerEvent* event = new PlayerEvent();
	char eventbuffer[size];

	recv(sockID,eventbuffer,size,MSG_EOR);

	stringstream eventstream;
	eventstream << eventbuffer;
	eventstream >> *event;

	return event;

}


ComunicationUtils::~ComunicationUtils() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
