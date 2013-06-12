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
	nextAvailableTeam = 2;
	scorage[0] = 0;
	scorage[1] = 0;
	scorage[2] = 0;
	regen = 0;
}

MissionManager::~MissionManager() {
}

bool MissionManager::hasEndedFlagCapture(list<Player*> players,
		MobileEntity* flag) {

	list<Player*>::const_iterator iterator;
	Player* actualPlayer;

	for (iterator = players.begin(); iterator != players.end(); ++iterator) {
		actualPlayer = (Player*) (*iterator);
		if (actualPlayer->isDead()) {
			scorage[actualPlayer->getKilledBy()]++;
			scorage[actualPlayer->getTeam()]--;
			actualPlayer->respawn();
		} else {
			actualPlayer->resetRespawnTimer();
		}
	}

	if (flag != NULL) {
		if (flag->isDead()) {
			winningTeam = flag->getKilledBy();
			scorage[winningTeam] += 15;
			return true;
		}
	}

	return false;
}

int MissionManager::getNextAvailableTeam() {
	if (nextAvailableTeam == 1)
		nextAvailableTeam++;
	else
		nextAvailableTeam = 1;
	return nextAvailableTeam;
}

bool MissionManager::hasEndedTeamFight(list<Player*> players) {

	bool someoneAliveFirstTeam = false;
	bool someoneAliveSecondTeam = false;

	scorage[1] = 0;
	scorage[2] = 0;

	int firstTeamPlayers = 0;
	int secondTeamPlayers = 0;

	list<Player*>::const_iterator iterator;
	Player* actualPlayer;

	for (iterator = players.begin(); iterator != players.end(); ++iterator) {

		actualPlayer = (Player*) (*iterator);

		if (actualPlayer->getTeam() == 1) {
			firstTeamPlayers++;
			if (!actualPlayer->isDead()) {
				someoneAliveFirstTeam = true;
			} else {
				scorage[actualPlayer->getTeam()]--;
			}
		}

		if (actualPlayer->getTeam() == 2) {
			secondTeamPlayers++;
			if (!actualPlayer->isDead()) {
				someoneAliveSecondTeam = true;
			} else {
				scorage[actualPlayer->getTeam()]--;
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

bool MissionManager::hasEndedSuddenDeath(list<Player*> players) {
// En principio seria igual a team vs team.
// Cambiaria como spawnean cosas, pero no esta aca.
	return hasEndedTeamFight(players);
}

bool MissionManager::hasEndedGame(list<Player*> players, MobileEntity* flag) {
	regen += 0.05;
	if (regen >= 1) {
		regen = 0;
		regenLife(players);
	}
	if (typeOfMission == 1) {
		return hasEndedFlagCapture(players, flag);
	} else if (typeOfMission == 2) {
		return hasEndedTeamFight(players);
	} else if (typeOfMission == 3) {
		return hasEndedSuddenDeath(players);
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

void MissionManager::setMission(string mission) {
	if (missionTypes.find(mission) == missionTypes.end()) {
		setMission("TeamFight");
	} else {
		setTypeOfMission(missionTypes[mission]);
	}
}

int MissionManager::getScore(int team) {
	if (scorage.find(team) == scorage.end()) {
		return 0;
	} else {
		return scorage[team];
	}
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

void MissionManager::regenLife(list<Player*> players) {
	list<Player*>::const_iterator iterator;
	Player* actualPlayer;

	for (iterator = players.begin(); iterator != players.end(); ++iterator) {

		actualPlayer = (Player*) (*iterator);

		if (actualPlayer->getLife() < 100)
			actualPlayer->setLife(actualPlayer->getLife() + 1);
		if (actualPlayer->getMagic() < 100)
			actualPlayer->setMagic(actualPlayer->getMagic() + 1);

	}
}
