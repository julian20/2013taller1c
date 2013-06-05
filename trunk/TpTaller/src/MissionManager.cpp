/*
 * MissionManager.cpp
 *
 *  Created on: May 25, 2013
 *      Author: gonchub
 */

#include <MissionManager.h>

using namespace std;

MissionManager::MissionManager() {
	missionTypes["FlagCapture"] = 1;
	missionTypes["TeamFight"] = 2;
	missionTypes["SuddenDeath"] = 3;
	typeOfMission = 2;
	winningTeam = 0;
	nextAvailableTeam = 2;
}

MissionManager::~MissionManager() {
}

bool MissionManager::hasEndedFlagCapture(list<Player*> mobileEntities) {

	list<Player*>::const_iterator iterator;
	Player* actualMobileEntity;
	for (iterator = mobileEntities.begin(); iterator != mobileEntities.end();
			++iterator) {

		actualMobileEntity = (Player*) (*iterator);

		// La flag es la unica entidad con team = 3.
		if (actualMobileEntity->getTeam() == 3) {
			if (actualMobileEntity->isDead()) {
				winningTeam = actualMobileEntity->getKilledBy();
				return true;
			}
		}
	}

	return false;
}

int MissionManager::getNextAvailableTeam(){
	if (nextAvailableTeam == 1)
		nextAvailableTeam++;
	else
		nextAvailableTeam = 1;
	return nextAvailableTeam;
}

bool MissionManager::hasEndedTeamFight(list<Player*> mobileEntities) {

	bool someoneAliveFirstTeam = false;
	bool someoneAliveSecondTeam = false;

	int firstTeamPlayers = 0;
	int secondTeamPlayers = 0;

	list<Player*>::const_iterator iterator;
	Player* actualMobileEntity;

	for (iterator = mobileEntities.begin(); iterator != mobileEntities.end();
			++iterator) {

		actualMobileEntity = (Player*) (*iterator);

		if (actualMobileEntity->getTeam() == 1) {
			firstTeamPlayers++;
			if (!actualMobileEntity->isDead()) {
				someoneAliveFirstTeam = true;
			}
		}

		if (actualMobileEntity->getTeam() == 2) {
			secondTeamPlayers++;
			if (!actualMobileEntity->isDead()) {
				someoneAliveSecondTeam = true;
			}
		}

	}

	// No se conecto ningun jugador del equipo contrario.
	if (firstTeamPlayers < 1 || secondTeamPlayers < 1)
		return false;

	// Caso muy borde.
	if (!someoneAliveFirstTeam && !someoneAliveSecondTeam) {
		winningTeam = 0;
		return true;
	}

	if (!someoneAliveSecondTeam) {
		winningTeam = 1;
		return true;
	}

	if (!someoneAliveFirstTeam) {
		winningTeam = 2;
		return true;
	}

	return false;
}

bool MissionManager::hasEndedSuddenDeath(list<Player*> mobileEntities) {
	// En principio seria igual a team vs team.
	// Cambiaria como spawnean cosas, pero no esta aca.
	return hasEndedTeamFight(mobileEntities);
}

bool MissionManager::hasEndedGame(list<Player*> mobileEntities) {
	if (typeOfMission == 1) {
		return hasEndedFlagCapture(mobileEntities);
	} else if (typeOfMission == 2) {
		return hasEndedTeamFight(mobileEntities);
	} else if (typeOfMission == 3) {
		return hasEndedSuddenDeath(mobileEntities);
	} else {
		return false;
	}
}

int MissionManager::getTypeOfMission() {
	return typeOfMission;
}

void MissionManager::setTypeOfMission(int type) {
	typeOfMission = type;
}

map<string, int> MissionManager::getMissionTypes() {
	return missionTypes;
}

void MissionManager::setMissionTypes(map<string, int> missionMap) {
	missionTypes = missionMap;
}

int MissionManager::getWinningTeam() {
	return winningTeam;
}

void MissionManager::setWinningTeam(int team) {
	winningTeam = team;
}

