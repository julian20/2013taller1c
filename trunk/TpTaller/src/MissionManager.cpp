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

bool MissionManager::hasEndedFlagCapture() {
	// TODO: recibiria la Mobile entity que representa la flag.
	/*
	 * if( flag->isDead() ) {
	 * 		winningTeam = flag->getKilledBy();
	 * 		return true;
	 * }
	 */
	return false;
}

bool MissionManager::hasEndedTeamFight() {
	// TODO: recibiria la lista de todos los players controlados del juego.
	/*
	 * vector<Player*> firstTeam;
	 * vector<Player*> secondTeam;
	 *
	 * Player* actualPlayer = NULL;
	 *
	 * for( unsigned i = 0 ; i < playerList.size() ; i++ ) {
	 * 		actualPlayer = playerList[i];
	 * 		if( actualPlayer->getTeam() == 1 && ! actualPlayer->isDead() ) {
	 * 			firstTeam.push_back(actualPlayer);
	 * 		} else if ( actualPlayer->getTeam() == 2  && ! actualPlayer->isDead() ) {
	 * 			secondTeam.push_back(actualPlayer);
	 * 		} else {
	 * 			cout << "ERROR: Player doesnt have team" << endl;
	 * 		}
	 * }
	 *
	 * // Se podria ver el caso raro y borde en el que ambos teams tengan
	 * // 0 integrantes vivos, porque a todos los mato una mob. Lo dejo
	 * // como TODO.
	 * if( firstTeam.size() == 0 ) {
	 * 		winningTeam = 2;
	 * 		return true;
	 * 	} else if( firstTeam.size() == 0 ) {
	 * 		winningTeam = 1;
	 * 		return true;
	 * 	} else {
	 *		return false;
	 * 		cout << "Game continues" << endl;
	 * 	}
	 *
	 */
	return false;
}

bool MissionManager::hasEndedSuddenDeath() {
	// En principio seria igual a team vs team.
	// Cambiaria como spawnean cosas, pero no esta aca.
	return hasEndedTeamFight();
}

bool MissionManager::hasEndedGame() {
	if (typeOfMission == 1) {
		return hasEndedFlagCapture();
	} else if (typeOfMission == 2) {
		return hasEndedTeamFight();
	} else if (typeOfMission == 3) {
		return hasEndedSuddenDeath();
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
