/*
 * ComunicationUtils.h
 *
 *  Created on: 01/05/2013
 *      Author: tomas
 */

#ifndef COMUNICATIONUTILS_H_
#define COMUNICATIONUTILS_H_

#include <networking/PlayerEvent.h>
#include <networking/PlayerInfo.h>
#include <networking/PlayerUpdate.h>

#include <string>

namespace std {

class ComunicationUtils {
public:
	static void 		sendNumber(int sockID,int number);
	static int  		recvNumber(int sockID);

	static void 		sendString(int sockID,string string);
	static string		recvString(int sockID);

	static void 		sendPlayerInfo(int sockID,PlayerInfo* info);
	static PlayerInfo* 	recvPlayerInfo(int sockID);

	static void 		sendPlayerEvent(int sockID,PlayerEvent* event);
	static PlayerEvent* recvPlayerEvent(int sockID);

	static void			sendPlayerUpdate(int sockID,PlayerUpdate* update);
	static PlayerUpdate* recvPlayerUpdate(int sockID);

	static void 		sendChatUpdate(int sockID,ChatUpdate* update);
	static ChatUpdate* recvChatUpdate(int sockID);

	static void downloadFile(int clientID);
	static void sendFile(string fileOriginName, string fileDestName, int sockID);

	static void sendChat(int sockID, Chat* chat);
	static Chat* recvChat(int sockID);

private:
	ComunicationUtils();
	virtual ~ComunicationUtils();
};

} /* namespace std */
#endif /* COMUNICATIONUTILS_H_ */
