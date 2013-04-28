/*
 * Server.h
 *
 *  Created on: 26/04/2013
 *      Author: tomas
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <networking/GlobalChanges.h>


namespace std {



class Server {
public:
	Server(string host, int port);
	void run();
	GlobalChanges* getChanges();
	void sendMap(string mapfile,int sockID);
	PlayerInfo* recievePlayerInfo(int clientSocket);
	void sendGlobalChanges(int sockID, GlobalChanges* changes);
	virtual ~Server();
private:
	int serverID;
	GlobalChanges* changes;
};

typedef struct aux{
	Server* server;
	int clientID;
}ThreadParameter;

} /* namespace std */
#endif /* SERVER_H_ */
