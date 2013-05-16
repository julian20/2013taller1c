/*
 * ComunicationUtils.cpp
 *
 *  Created on: 01/05/2013
 *      Author: tomas
 */

#include <networking/ComunicationUtils.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <iostream>
#include <sstream>
#include <fstream>

#define ITER_LIMIT 10

namespace std {

#define NUMBER_SIZE 10
#define EXTRA 3
#define READING_SIZE 8096

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

void ComunicationUtils::downloadFile(int clientID) {

	// Read Picture Size
	int size = ComunicationUtils::recvNumber(clientID);
	cout << "Receiving file of size: "<<size<<endl;
	// Read filename
	string filename = ComunicationUtils::recvString(clientID);
	cout << "Filename: "<<filename<<endl;
	char* fileBaseDir = strdup(filename.c_str());

	FILE *image;
	string dirName = string(dirname(fileBaseDir));
	string makeDir = string("mkdir -p ");
	system(string(makeDir + dirName).c_str());

	string outputFile(filename);
	image = fopen(outputFile.c_str(), "wb");
	/*if (image == NULL){
		cerr << "Error al abrir archivo: " << outputFile << ". Se ignorara." << endl;
		ComunicationUtils::sendNumber(clientID, ERROR);
		return;
	}
*/
	int recved = 0;
	unsigned char buffer[READING_SIZE];
	cout << "Starting download..."<<endl;
	while (recved < size){

		int sendedSize = ComunicationUtils::recvNumber(clientID);

		int readSize = 0;
		while (readSize != sendedSize){
			memset(buffer,-1,READING_SIZE);
			readSize = read(clientID,buffer,sendedSize);
		}

		recved += sendedSize;

		fwrite(buffer,sizeof(char),readSize,image);
	}

	fseek(image,0,SEEK_END);

	cout << "File size " << ftell(image) <<endl;

	cout << "Download ok? (1=ok): "<< (size == ftell(image)) <<endl;
	fclose(image);

}

void ComunicationUtils::sendFile(string fileOrigin, string fileDest, int sockID) {

	// Get Picture Size
	FILE *picture;
	picture = fopen(fileOrigin.c_str(), "r");
	if (!picture)
		cout << "ERROR OPEN FILE "<< fileOrigin<<endl;
	int size;
	fseek(picture, 0, SEEK_END);
	size = ftell(picture);
	fseek(picture, 0, SEEK_SET);

	// Send Picture Size
	ComunicationUtils::sendNumber(sockID,size);
	cout << "Sending file of size: "<<size<<endl;

	ComunicationUtils::sendString(sockID,fileDest);
	cout << "Filename: "<<fileDest<<endl;

	// Send Picture as Byte Array
	char buffer[READING_SIZE];
	cout << "Starting to send..."<<endl;
	int sentSize=0;
	while (!feof(picture)){

		memset(buffer,-1,READING_SIZE);

		int readSize = fread(buffer,sizeof(char),READING_SIZE,picture);

		ComunicationUtils::sendNumber(sockID, readSize);

		int sendSize = 0;
		while (sendSize != readSize){
			sendSize = send(sockID,buffer,readSize,0);
		}
		sentSize+=sendSize;

	}

	cout << "Sent size: "<<sentSize<<endl;

	fclose(picture);

}

ComunicationUtils::~ComunicationUtils() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
