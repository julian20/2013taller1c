/*
 * ServerMapPersistentConfiguration.h
 *
 *  Created on: May 11, 2013
 *      Author: gonchub
 */

#ifndef SERVERMAPPERSISTENTCONFIGURATION_H_
#define SERVERMAPPERSISTENTCONFIGURATION_H_

#include <model/entities/Entity.h>
#include <model/entities/player/Player.h>
#include <model/map/TextureHolder.h>
#include <model/map/MapData.h>
#include <view/configuration/GameConfiguration.h>
#include <view/EntityViewMap.h>
#include <view/entities/PlayerView.h>

#include <vector>

class ServerMapPersistentConfiguration {
public:
	ServerMapPersistentConfiguration();
	virtual ~ServerMapPersistentConfiguration();
	TextureHolder* getTextureHolder();
	void setTextureHolder(TextureHolder* textureHolder);
	MapData* getMapData();
	void setMapData(MapData* mapData);
	GameConfiguration* getAnimationConfiguration();
	void setAnimationConfiguration(GameConfiguration* animationConfig);
	EntityViewMap* getEntityViewMap();
	void setEntityViewMap(EntityViewMap* entityViewMap);

private:
	TextureHolder* textureHolder;
	MapData* mapData;
	GameConfiguration* animationConfiguration;
	EntityViewMap* entityViewMap;
};

#endif /* SERVERMAPPERSISTENTCONFIGURATION_H_ */
