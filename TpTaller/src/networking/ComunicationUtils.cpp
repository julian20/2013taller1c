/*
 * ComunicationUtils.cpp
 *
 *  Created on: 01/05/2013
 *      Author: tomas
 */

#include <networking/ComunicationUtils.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>


#define ITER_LIMIT 10

using namespace std;

#define NUMBER_SIZE 16
#define EXTRA 3
#define READING_SIZE 1024

ComunicationUtils::ComunicationUtils() {
}

void ComunicationUtils::sendNumber(int sockID,int number){

	stringstream nstream;

	nstream << number;

	send(sockID,nstream.str().c_str(),NUMBER_SIZE,0);


}

int ComunicationUtils::recvNumber(int sockID){

	char nbuffer[NUMBER_SIZE];
	int n = -1;

	int recvSize = 0;
	int i = 0;

	while (recvSize != NUMBER_SIZE){
		recvSize = recv(sockID,nbuffer,NUMBER_SIZE, 0);
		i++;
		if (i > ITER_LIMIT) break;

	}

	stringstream nstream;
	nstream << nbuffer;
	nstream >> n;

	return n;

}

void ComunicationUtils::sendChat(int sockID, Chat* chat){
	stringstream chatstream;
	chatstream << *(chat);

	int size = chatstream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,chatstream.str().c_str(),size, MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}
}
Chat* ComunicationUtils::recvChat(int sockID){

	int size = ComunicationUtils::recvNumber(sockID);
	Chat* chat = new Chat();
	char chatbuffer[size];

	int recvSize = 0;
	int i = 0;

	while (recvSize != size){
		recvSize = recv(sockID,chatbuffer,size,MSG_EOR);
		i++;
		if (i > ITER_LIMIT) break;
	}

	stringstream chatstream;
	chatstream << chatbuffer;
	chatstream >> *chat;

	return chat;

}
void ComunicationUtils::sendString(int sockID,string string){

	stringstream sstream;
	sstream << string;

	int size = string.size() + EXTRA;

	ComunicationUtils::sendNumber(sockID,size);

	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,sstream.str().c_str(),size,0);
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
		recvSize = recv(sockID,sbuffer,size,0);
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
		sendSize = send(sockID,infostream.str().c_str(),size, 0);
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
		recvSize = recv(sockID,infobuffer,size,0);
		i++;
		if (i > ITER_LIMIT) break;
	}

	stringstream infostream;
	infostream << infobuffer;

	infostream >> *info;

	return info;

}


void ComunicationUtils::sendMobileEntityInfo(int sockID,MobileEntityInfo* info){

	stringstream infostream;
	infostream << *(info);

	int size = infostream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

	// SEND MOB INFO
	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,infostream.str().c_str(),size, 0);
		i++;
		if (i > ITER_LIMIT) break;
	}
}

MobileEntityInfo* ComunicationUtils::recvMobileEntityInfo(int sockID){

	int size = ComunicationUtils::recvNumber(sockID);

	char infobuffer[size];

	int recvSize = 0;
	int i = 0;

	while (recvSize != size){
		recvSize = recv(sockID,infobuffer,size,0);
		i++;
		if (i > ITER_LIMIT) break;
	}

	stringstream infostream;
	infostream << infobuffer;

	MobileEntityInfo* info = new MobileEntityInfo();

	infostream >> *info;

	return info;
}

void ComunicationUtils::sendEntityInfo(int sockID,EntityInfo* info){

	stringstream infostream;
	infostream << *(info);

	int size = infostream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

	// SEND MOB INFO
	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,infostream.str().c_str(),size, 0);
		i++;
		if (i > ITER_LIMIT) break;
	}
}

EntityInfo* ComunicationUtils::recvEntityInfo(int sockID){

	int size = ComunicationUtils::recvNumber(sockID);

	char infobuffer[size];

	int recvSize = 0;
	int i = 0;

	while (recvSize != size){
		recvSize = recv(sockID,infobuffer,size,0);
		i++;
		if (i > ITER_LIMIT) break;
	}

	stringstream infostream;
	infostream << infobuffer;

	EntityInfo* info = new EntityInfo();

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
		sendSize = send(sockID,eventstream.str().c_str(),size, 0);
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
		recvSize = recv(sockID,eventbuffer,size,0);
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

	send(sockID,updatestream.str().c_str(), size, 0);

}
void ComunicationUtils::sendChatMessage(int sockID, ChatMessage* msj)
{
	stringstream updatestream;
	updatestream << *msj;

	int size = updatestream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

		int sendSize = 0;
		int i = 0;

		while (sendSize != size)
		{
			sendSize = send(sockID,updatestream.str().c_str(), size, 0);
			i++;
			if (i > ITER_LIMIT) break;
		}
}

ChatMessage* ComunicationUtils::recvChatMessage(int sockID) {
	int size = ComunicationUtils::recvNumber(sockID);
	char updatebuffer[size];
	stringstream updatestream;

	int recvSize = 0;
	int i = 0;

	while (recvSize != size){
		recvSize = recv(sockID,updatebuffer,size,0);
		i++;
		if (i > ITER_LIMIT) break;
	}

	updatestream << updatebuffer;

	ChatMessage* update = new ChatMessage();

	updatestream >> *update;

	return update;
}

void ComunicationUtils::sendPlayerUpdate(int sockID,PlayerUpdate* update){

	stringstream updatestream;
	updatestream << *update;

	int size = updatestream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,updatestream.str().c_str(), size, 0);
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
		recvSize = recv(sockID,updatebuffer,size,0);
		i++;
		if (i > ITER_LIMIT) break;
	}

	updatestream << updatebuffer;

	PlayerUpdate* update = new PlayerUpdate();

	updatestream >> *update;

	return update;

}

void ComunicationUtils::sendMobileEntityUpdate(int sockID,MobileEntityUpdate* update){

	stringstream updatestream;
	updatestream << *update;

	int size = updatestream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,updatestream.str().c_str(), size, 0);
		i++;
		if (i > ITER_LIMIT) break;
	}
}

MobileEntityUpdate* ComunicationUtils::recvMobileEntityUpdate(int sockID){

	int size = ComunicationUtils::recvNumber(sockID);
	char updatebuffer[size];
	stringstream updatestream;

	int recvSize = 0;
	int i = 0;

	while (recvSize != size){
		recvSize = recv(sockID,updatebuffer,size,0);
		i++;
		if (i > ITER_LIMIT) break;
	}

	updatestream << updatebuffer;

	MobileEntityUpdate* update = new MobileEntityUpdate();

	updatestream >> *update;

	return update;

}

ChatUpdate* ComunicationUtils::recvChatUpdate(int sockID){

	int size = ComunicationUtils::recvNumber(sockID);
	char updatebuffer[size];
	stringstream updatestream;

	recv(sockID,updatebuffer,size,0);

	updatestream << updatebuffer;

	ChatUpdate* update = new ChatUpdate();

	updatestream >> *update;

	return update;

}

void ComunicationUtils::downloadFile(int clientID) {

	// Read Picture Size
	int size = ComunicationUtils::recvNumber(clientID);
	cout << "Receiving file of size: " << size << endl;
	// Read filename
	string filename = ComunicationUtils::recvString(clientID);
	cout << "Filename: " << filename << endl;
	char* fileBaseDir = strdup(filename.c_str());
	char* fileDir = strdup(filename.c_str());

    string dirName = string(dirname(fileDir));

    string makeDir = string("mkdir -p ");
    system(string(makeDir + dirName).c_str());

	FILE* file = fopen(fileBaseDir, "w");

	// Send Picture as Byte Array
	char* buffer = (char*)malloc(size);
	int aux = recv(clientID,buffer,size,0);
	fwrite(buffer,1,aux,file);
	while(aux<size){
		int temp = recv(clientID,buffer,size-aux,0);
		aux +=temp;
		fwrite(buffer,1,temp,file);
	}

	cout << "Received size: "<<ftell(file)<<endl;
	free(buffer);
	fclose(file);

}


void ComunicationUtils::sendFile(string fileOrigin, string fileDest, int sockID) {

	// Get Picture Size
	int file;
	file = open(fileOrigin.c_str(), O_RDONLY);

	struct stat stat_buf;
	fstat(file, &stat_buf);


	// Send Picture Size
	ComunicationUtils::sendNumber(sockID,stat_buf.st_size);
	cout << "Sending file of size: "<<stat_buf.st_size<<endl;

	ComunicationUtils::sendString(sockID,fileDest);
	cout << "Filename: "<<fileDest<<endl;

	// Send Picture as Byte Array

	cout << "Starting to send..."<<endl;
	int aux =sendfile (sockID,file, NULL, stat_buf.st_size);
	while(aux<stat_buf.st_size){
		aux +=sendfile (sockID,file, NULL, stat_buf.st_size-aux);
	}

	cout << "Sent size: "<<aux<<endl;

	close(file);

}


void ComunicationUtils::sendMobUpdate(int sockID, MobUpdate* update){

	stringstream updatestream;
	updatestream << *update;

	int size = updatestream.str().size() + EXTRA;
	ComunicationUtils::sendNumber(sockID,size);

	int sendSize = 0;
	int i = 0;

	while (sendSize != size){
		sendSize = send(sockID,updatestream.str().c_str(), size, 0);
		i++;
		if (i > ITER_LIMIT) break;
	}
}

MobUpdate* ComunicationUtils::recvMobUpdate(int sockID){
	int size = ComunicationUtils::recvNumber(sockID);
	char updatebuffer[size];
	stringstream updatestream;

	int recvSize = 0;
	int i = 0;

	while (recvSize != size){
		recvSize = recv(sockID,updatebuffer,size,0);
		i++;
		if (i > ITER_LIMIT) break;
	}

	updatestream << updatebuffer;

	MobUpdate* update = new MobUpdate();

	updatestream >> *update;

	return update;
}

ComunicationUtils::~ComunicationUtils() {
}

