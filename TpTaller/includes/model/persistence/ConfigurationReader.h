/* 
 * File:   ConfigurationReader.h
 * Author: gonchub
 *
 * Created on March 21, 2013, 9:25 PM
 */

#ifndef CONFIGURATIONREADER_H
#define	CONFIGURATIONREADER_H

#include <yaml-cpp/yaml.h>
#include <model/Vector2.h>
#include <model/entityProperties/Speed.h>
#include <model/entityProperties/Position.h>
#include <model/entityProperties/Power.h>
#include <model/persistence/ConfigurationReader.h>
#include <model/persistence/ServerMapPersistentConfiguration.h>
#include <model/persistence/ClientPlayerPersistentConfiguration.h>
#include <model/entities/Entity.h>
#include <model/entities/MobileEntity.h>
#include <model/entities/player/Player.h>
#include <model/entities/Flag.h>
#include <model/entities/Items/Item.h>
#include <model/entities/Items/LifeHeart.h>
#include <model/entities/Items/MagicBottle.h>
#include <model/entities/Items/Lantern.h>
#include <model/entities/Items/Shield.h>
#include <model/entities/Items/MapItem.h>
#include <model/entities/Items/Chest.h>
#include <model/entities/Items/StrengthBuff.h>
#include <model/entities/Items/EarthquakeItem.h>
#include <model/entities/Items/CrystalBallItem.h>
#include <model/entities/Items/SpellShieldItem.h>
#include <model/entities/Items/FreezeSpellItem.h>
#include <model/entities/Items/GolemSpellItem.h>
#include <model/map/Tile.h>
#include <model/map/TextureDefinition.h>
#include <model/map/TextureHolder.h>
#include <model/persistence/PersistentConfiguration.h>
#include <view/configuration/GameConfiguration.h>
#include <view/EntityViewMap.h>
#include <view/entities/EntityViewData.h>
#include <view/entities/EntityHolder.h>
#include <view/entities/EntityView.h>
#include <view/entities/PlayerView.h>
#include <view/entities/MobileEntityView.h>
#include <view/entities/ItemViewHolder.h>
#include <view/entities/MobileEntityViewHolder.h>
#include <model/Logs/Logs.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <cstdlib>
#include <stdlib.h>

using namespace std;

class ConfigurationReader {
public:
	ConfigurationReader();
	ConfigurationReader(const ConfigurationReader& orig);
	PersistentConfiguration loadConfiguration(std::string configurationFile,
			std::string outputFilename);
	ServerMapPersistentConfiguration loadServerMapConfiguration(
			std::string configurationFile);
	ClientPlayerPersistentConfiguration loadClientPlayerConfiguration(
			std::string configurationFile);
	virtual ~ConfigurationReader();
private:
};

#endif	/* CONFIGURATIONREADER_H */

