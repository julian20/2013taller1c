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
#include <networking/MobileEntityInfo.h>
#include <networking/MobUpdate.h>

#include <string>

using namespace std;

class ComunicationUtils {
public:
	static void 		sendNumber(int sockID,int number);
	static int  		recvNumber(int sockID);

	static void 		sendString(int sockID,string string);
	static string		recvString(int sockID);

	static void 		sendPlayerInfo(int sockID,PlayerInfo* info);
	static PlayerInfo* 	recvPlayerInfo(int sockID);

	static void 		sendMobileEntityInfo(int sockId,MobileEntityInfo* info);
	static MobileEntityInfo*		recvMobileEntityInfo(int sockId);

	static void 		sendPlayerEvent(int sockID,PlayerEvent* event);
	static PlayerEvent* recvPlayerEvent(int sockID);

	static void			sendPlayerUpdate(int sockID,PlayerUpdate* update);
	static PlayerUpdate* recvPlayerUpdate(int sockID);

	static void			sendMobileEntityUpdate(int sockID,MobileEntityUpdate* update);
	static MobileEntityUpdate* recvMobileEntityUpdate(int sockID);

	static void 		sendChatUpdate(int sockID,ChatUpdate* update);
	static ChatUpdate* recvChatUpdate(int sockID);

	static void downloadFile(int clientID);
	static void sendFile(string fileOriginName, string fileDestName, int sockID);

	static void sendChat(int sockID, Chat* chat);
	static Chat* recvChat(int sockID);

	static void sendChatMessage(int sockID, ChatMessage* chatmsj);
	static ChatMessage* recvChatMessage(int sockID);

	static void         sendMobUpdate(int sockID, MobUpdate* update);
	static MobUpdate*   recvMobUpdate(int sockID);

private:
	ComunicationUtils();
	virtual ~ComunicationUtils();
};

#endif /* COMUNICATIONUTILS_H_ */
