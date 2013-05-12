/*
 * ClientPlayerPersistentConfiguration.h
 *
 *  Created on: May 11, 2013
 *      Author: gonchub
 */

#ifndef CLIENTPLAYERPERSISTENTCONFIGURATION_H_
#define CLIENTPLAYERPERSISTENTCONFIGURATION_H_

#include <model/entities/Entity.h>
#include <model/entities/player/Player.h>
#include <view/configuration/GameConfiguration.h>
#include <view/entities/PlayerView.h>

class ClientPlayerPersistentConfiguration {
public:
	ClientPlayerPersistentConfiguration();
	virtual ~ClientPlayerPersistentConfiguration();
	std::vector<PlayerView*> getPlayerViewList();
	GameConfiguration* getGameConfiguration();
	void setPlayerViewList(std::vector<PlayerView*> playerViewList);
	void setGameConfiguration(GameConfiguration* gameConfiguration);
private:
	GameConfiguration* gameConfiguration;
	std::vector<PlayerView*> playerViewList;
};

#endif /* CLIENTPLAYERPERSISTENTCONFIGURATION_H_ */
