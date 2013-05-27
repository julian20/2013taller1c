/*
 * SinglePlayerServer.h
 *
 *  Created on: 27/05/2013
 *      Author: tomas
 */

#ifndef SINGLEPLAYERSERVER_H_
#define SINGLEPLAYERSERVER_H_

#include <networking/Server.h>

namespace std {

class SinglePlayerServer : public Server {
public:
	SinglePlayerServer(int port);

	void run(MultiplayerGame* game);
	void handleConnection();

	int getServerID();
	void setClientID(int clientID);

	virtual ~SinglePlayerServer();

private:
	int clientID;

	pthread_t gameThread;
	pthread_t serverThread;
};

} /* namespace std */
#endif /* SINGLEPLAYERSERVER_H_ */
