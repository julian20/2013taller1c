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

class MissionManager {
public:
	MissionManager();
	virtual ~MissionManager();

	// Methods
	bool hasEndedFlagCapture(list<Player*> mobileEntities);
	bool hasEndedTeamFight(list<Player*> mobileEntities);
	bool hasEndedSuddenDeath(list<Player*> mobileEntities);
	bool hasEndedGame(list<Player*> mobileEntities);

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
