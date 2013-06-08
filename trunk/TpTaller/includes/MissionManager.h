/*
 * MissionManager.h
 *
 *  Created on: May 25, 2013
 *      Author: gonchub
 */

#ifndef MISSIONMANAGER_H_
#define MISSIONMANAGER_H_

#include <vector>
#include <string>
#include <map>

using namespace std;

#include <model/entities/player/Player.h>
#include <model/entities/Flag.h>

class MissionManager {
public:
	MissionManager();
	virtual ~MissionManager();

	// Methods
	bool hasEndedFlagCapture(list<Player*> players, Flag* flag);
	bool hasEndedTeamFight(list<Player*> players);
	bool hasEndedSuddenDeath(list<Player*> players);
	bool hasEndedGame(list<Player*> players, Flag* flag);

	// Getters and Setters
	int getTypeOfMission();
	int getNextAvailableTeam();
	int getWinningTeam();
	int getScore(int team);
	map<string, int> getMissionTypes();
	void setTypeOfMission(int type);
	void setWinningTeam(int team);
	void setMissionTypes(map<string, int> missionMap);
	void setMission(string mission);

private:
	int typeOfMission, nextAvailableTeam;
	int winningTeam;
	map<int, int> scorage;
	map<string, int> missionTypes;
};

#endif /* MISSIONMANAGER_H_ */
