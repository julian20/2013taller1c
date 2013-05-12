/*
 * ClientPlayerPersistentConfiguration.cpp
 *
 *  Created on: May 11, 2013
 *      Author: gonchub
 */

#include <model/persistence/ClientPlayerPersistentConfiguration.h>

ClientPlayerPersistentConfiguration::ClientPlayerPersistentConfiguration() {
	this->playerViewList = NULL;
	this->gameConfiguration = NULL;
}

ClientPlayerPersistentConfiguration::~ClientPlayerPersistentConfiguration() {
	delete this->playerViewList;
	delete this->gameConfiguration;
}

std::vector<PlayerView*> ClientPlayerPersistentConfiguration::getPlayerViewList() {
	return this->playerViewList;
}

void ClientPlayerPersistentConfiguration::setPlayerViewList(std::vector<PlayerView*>  playerViewList) {
	this->playerViewList = playerViewList;
}

GameConfiguration* ClientPlayerPersistentConfiguration::getGameConfiguration() {
	return this->gameConfiguration;
}

void ClientPlayerPersistentConfiguration::setGameConfiguration(GameConfiguration* gameConfiguration) {
	this->gameConfiguration = gameConfiguration;
}
