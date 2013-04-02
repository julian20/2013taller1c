/*
 * PersistentConfiguration.h
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#ifndef PERSISTENTCONFIGURATION_H_
#define PERSISTENTCONFIGURATION_H_

#include <model/entities/Entity.h>
#include <model/entities/personaje/Personaje.h>
#include <model/map/TextureHolder.h>
#include <model/map/MapData.h>

#include <vector>

class PersistentConfiguration {
public:
	PersistentConfiguration();
	virtual ~PersistentConfiguration();
	TextureHolder* getTextureHolder();
	void setTextureHolder(TextureHolder* textureHolder);
	std::vector<Personaje*> getPersonajeList();
	void setPersonajeList(std::vector<Personaje*> entityList);
	MapData* getMapData();
	void setMapData(MapData* mapData);

private:
	std::vector<Personaje*> entityList;
	TextureHolder* textureHolder;
	MapData* mapData;
};

#endif /* PERSISTENTCONFIGURATION_H_ */
