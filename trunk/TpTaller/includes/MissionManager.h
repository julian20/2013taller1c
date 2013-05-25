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

class MissionManager {
public:
	MissionManager();
	virtual ~MissionManager();

	// Methods
	bool hasEndedFlagCapture();
	bool hasEndedTeamFight();
	bool hasEndedSuddenDeath();
	bool hasEndedGame();

	// Getters and Setters
	int getTypeOfMission();
	int getWinningTeam();
	map<string, int> getMissionTypes();
	void setTypeOfMission(int type);
	void setWinningTeam(int team);
	void setMissionTypes(map<string, int> missionMap);

private:
	int typeOfMission;
	int winningTeam;
	map<string, int> missionTypes;
};

#endif /* MISSIONMANAGER_H_ */
