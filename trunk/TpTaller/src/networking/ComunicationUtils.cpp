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

#define NUMBER_SIZE 10*sizeof(char)
#define STRING_SIZE 150*sizeof(char)
#define INFO_SIZE 4*(sizeof(PlayerInfo) + sizeof(Player))
#define EVENT_SIZE 4*(sizeof(PlayerEvent) + sizeof(Coordinates))
#define UPDATE_SIZE 80
#define EXTRA 3

ComunicationUtils::ComunicationUtils() {
	// TODO Auto-generated constructor stub
}

void ComunicationUtils::sendNumber(int sockID,int number){
	char buffer[NUMBER_SIZE];
	sprintf(buffer,"%d",number);

	while(send(sockID,buffer,NUMBER_SIZE,MSG_EOR)!=NUMBER_SIZE){

		//Do nothing (send again)!

	}

}

int ComunicationUtils::recvNumber(int sockID){
	char buffer[NUMBER_SIZE];
	//Loop while we're not getting our shit

	while(recv(sockID,buffer,NUMBER_SIZE, MSG_EOR)!=NUMBER_SIZE);
	int n = atoi(buffer);

	return n;
}

void ComunicationUtils::sendString(int sockID,string string){

	int size = STRING_SIZE;
	stringstream sstream;
	sstream << string;

	while(send(sockID,sstream.str().c_str(),size,MSG_EOR)!=size);

}

string ComunicationUtils::recvString(int sockID){

	int size = STRING_SIZE;

	char sbuffer[size];

	while (recv(sockID,sbuffer,size,MSG_EOR)!=size);

	string srecv = string(sbuffer);

	return srecv;

}

void ComunicationUtils::sendPlayerInfo(int sockID,PlayerInfo* info){

	stringstream infostream;
	infostream << *(info);

	int size = INFO_SIZE;

	// SEND PLAYER INFO
	while(send(sockID,infostream.str().c_str(),size, MSG_EOR)!=size);

}

PlayerInfo* ComunicationUtils::recvPlayerInfo(int sockID){

	int size = INFO_SIZE;
	PlayerInfo* info = new PlayerInfo();

	char infobuffer[size];

	while(recv(sockID,infobuffer,size,MSG_EOR)!=size);
	stringstream infostream;
	infostream << infobuffer;

	infostream >> *info;

	return info;

}

void ComunicationUtils::sendPlayerEvent(int sockID,PlayerEvent* event){
	stringstream eventstream;
	eventstream << *(event);

	int size = EVENT_SIZE;

	while(send(sockID,eventstream.str().c_str(),size, MSG_EOR)!=size);

}

PlayerEvent* ComunicationUtils::recvPlayerEvent(int sockID){

	int size = EVENT_SIZE;
	PlayerEvent* event = new PlayerEvent();
	char eventbuffer[size];

	while(recv(sockID,eventbuffer,size,MSG_EOR)!=size);

	stringstream eventstream;
	eventstream << eventbuffer;
	eventstream >> *event;

	return event;

}

void ComunicationUtils::sendPlayerUpdate(int sockID,PlayerUpdate* update){

	int size = UPDATE_SIZE;
	stringstream updatestream;
	updatestream << *update;

	while(send(sockID,updatestream.str().c_str(), size, MSG_EOR)!=size);

}

PlayerUpdate* ComunicationUtils::recvPlayerUpdate(int sockID){

	int size = UPDATE_SIZE;
	char updatebuffer[size];
	stringstream updatestream;

	while(recv(sockID,updatebuffer,size,MSG_EOR)!=size);

	updatestream << updatebuffer;

	PlayerUpdate* update = new PlayerUpdate();

	updatestream >> *update;

	return update;

}


ComunicationUtils::~ComunicationUtils() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
