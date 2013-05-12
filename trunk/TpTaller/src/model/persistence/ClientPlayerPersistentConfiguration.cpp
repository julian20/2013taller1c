/*
 * ClientPlayerPersistentConfiguration.cpp
 *
 *  Created on: May 11, 2013
 *      Author: gonchub
 */

#include <model/persistence/ClientPlayerPersistentConfiguration.h>

ClientPlayerPersistentConfiguration::ClientPlayerPersistentConfiguration() {
	this->gameConfiguration = NULL;
}

ClientPlayerPersistentConfiguration::~ClientPlayerPersistentConfiguration() {

	for (unsigned int i = 0; i < this->playerViewList.size(); i++) {
		delete this->playerViewList[i];
	}

	delete this->gameConfiguration;
}

std::vector<PlayerView*> ClientPlayerPersistentConfiguration::getPlayerViewList() {
	return this->playerViewList;
}

void ClientPlayerPersistentConfiguration::setPlayerViewList(
		std::vector<PlayerView*> playerViewList) {
	this->playerViewList = playerViewList;
}

GameConfiguration* ClientPlayerPersistentConfiguration::getGameConfiguration() {
	return this->gameConfiguration;
}

void ClientPlayerPersistentConfiguration::setGameConfiguration(
		GameConfiguration* gameConfiguration) {
	this->gameConfiguration = gameConfiguration;
}
