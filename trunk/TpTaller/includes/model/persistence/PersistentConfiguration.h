/*
 * PersistentConfiguration.h
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#ifndef PERSISTENTCONFIGURATION_H_
#define PERSISTENTCONFIGURATION_H_

#include <model/entities/Entity.h>
#include <model/entities/player/Player.h>
#include <model/map/TextureHolder.h>
#include <model/map/MapData.h>
#include <view/configuration/GameConfiguration.h>
#include <view/EntityViewMap.h>
#include <view/entities/PlayerView.h>
#include <view/entities/MobileEntityView.h>
#include <view/entities/ItemViewHolder.h>
#include <view/entities/MobileEntityViewHolder.h>
#include <model/entities/enemies/Mob.h>

#include <vector>

class PersistentConfiguration {
public:
	PersistentConfiguration();
	virtual ~PersistentConfiguration();
	TextureHolder* getTextureHolder();
	void setTextureHolder(TextureHolder* textureHolder);
	std::vector<Player*> getPersonajeList();
	void setPersonajeList(std::vector<Player*> entityList);
	void setViewList(std::vector<PlayerView*> viewList);
	std::vector<PlayerView*> getViewList();
	void setMobileEntityViewList(std::vector<MobileEntityView*> viewList);
	std::vector<MobileEntityView*> getMobileEntityViewList();
	MapData* getMapData();
	void setMapData(MapData* mapData);
	GameConfiguration* getAnimationConfiguration();
	void setAnimationConfiguration(GameConfiguration* animationConfig);
	EntityViewMap* getEntityViewMap();
	void setEntityViewMap(EntityViewMap* entityViewMap);
	map<int,MobileEntity*> getMobileEntities();
	void setMobileEntities(std::vector<MobileEntity*> mobiles);
	std::vector<Item*> getItems();
	void setItems(std::vector<Item*> itemViews);
	ItemViewHolder* getItemViewHolder();
	void setItemViewHolder(ItemViewHolder* itemViewHolder);
	MobileEntityViewHolder* getMobileEntityViewHolder();
	void setMobileEntityViewHolder(MobileEntityViewHolder* mobileEntityViewHolder);
	void setMobileEntitiesView(std::vector<MobileEntityView*> list);
	std::vector<MobileEntityView*> getMobileEntitiesView();
	vector<MobileEntity*> getMobileEntityList();
	void setFlag(MobileEntity* flag);
	MobileEntity* getFlag();
private:
	std::vector<MobileEntityView*> allViewEntities;
	map<int,MobileEntity*> mobileEntityMap;
	std::vector<Player*> entityList;
	std::vector<MobileEntity*> mobs;
	std::vector<PlayerView*> viewList;
	std::vector<MobileEntityView*> mobileEntityViewList;
	std::vector<Item*> itemList;
	TextureHolder* textureHolder;
	MapData* mapData;
	GameConfiguration* animationConfiguration;
	EntityViewMap* entityViewMap;
	ItemViewHolder* itemViewHolder;
	MobileEntityViewHolder* mobileEntityViewHolder;
	MobileEntity* flag;
};

#endif /* PERSISTENTCONFIGURATION_H_ */
