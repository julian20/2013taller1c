/*
 * TileData.h
 *
 *  Created on: Apr 3, 2013
 *      Author: lucas
 */

#ifndef TILEDATA_H_
#define TILEDATA_H_

#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <model/entities/player/Player.h>
#include <vector>

using namespace std;

class TileData {
public:
	TileData();
	virtual ~TileData();

	//Getters
	std::string getType();
	MobileEntity* getAttackableMobileEntity();
	std::list<Entity*> getEntities();
	std::list<MobileEntity*> getMobileEntities();
	int getNumberOfEntitiesOnTile();
	bool getWasVisible();
	bool getIsVisible();
	bool isWalkable( bool ignorePlayer = false );

	//Setters
	void setType(std::string type);
	void addMobileEntity(MobileEntity* newPersonaje);
	void removeMobileEntity(MobileEntity* newMobileEntity);
	void setVisibility(bool value);
	void setWalkable(bool _walkable);

	void addEntity(Entity* entity);
	void removeEntity(Entity* entity);

private:
	std::string tileType;
	std::list<MobileEntity*> mobileEntities;
	std::list<Entity*> entities;
	std::list<Entity*>::iterator iterator;
	bool wasVisible;
	bool isVisible;
	bool walkable;
};

#endif /* TILEDATA_H_ */
