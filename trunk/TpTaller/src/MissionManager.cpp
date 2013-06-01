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
	typeOfMission = 1;
	winningTeam = 0;
}

MissionManager::~MissionManager() {
}

bool MissionManager::hasEndedFlagCapture(list<Player*> mobileEntities) {

	list<Player*>::const_iterator iterator;
	Player* actualMobileEntity;
	for (iterator = mobileEntities.begin(); iterator != mobileEntities.end();
			++iterator) {

		actualMobileEntity = (Player*) (*iterator);

		// TODO: ver como carajo se que es una flag si lo que tengo es mobile entity.
		// Una manera fea que se me ocurre es que la flag sea algo asi como el team 3.
		// Entonces preguntamos el team y listo, pero es feito feito.
		if (actualMobileEntity->getClassName() == "Flag") {

//			  if( actualMobileEntity->isDead() ) {
//			  		winningTeam = actualMobileEntity->getKilledBy();
//			  		return true;
//			  }

		}
	}

	return false;
}

bool MissionManager::hasEndedTeamFight(list<Player*> mobileEntities) {

	bool someoneAliveFirstTeam = false;
	bool someoneAliveSecondTeam = false;

	list<Player*>::const_iterator iterator;
	Player* actualMobileEntity;
	for (iterator = mobileEntities.begin(); iterator != mobileEntities.end();
			++iterator) {

		actualMobileEntity = (Player*) (*iterator);

		// TODO: ver como carajo se que es un player si lo que tengo es mobile entity.
		// Siguiendo lo que dije arriba, las entities tendria team. Team 1 y 2 serian
		// players, team 3 una flag, y team 0 los demas. Es feo, pero funcaria y no
		// habria que ver como preguntamos esto.
		//if (actualMobileEntity->getClassName() == "Player") {

//			if (actualMobileEntity->getTeam() == 1
//					&& actualMobileEntity->getLife() > 0) {
//				someoneAliveFirstTeam = true
//			} else if (actualMobileEntity->getTeam() == 2
//					&& actualMobileEntity->getLife() > 0) {
//				someoneAliveSecondTeam = true
//			}

		//}
	}

	if (someoneAliveFirstTeam == false) {
		winningTeam = 2;
		return true;
	} else if (someoneAliveSecondTeam == false) {
		winningTeam = 1;
		return true;
	}
	// Else
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

