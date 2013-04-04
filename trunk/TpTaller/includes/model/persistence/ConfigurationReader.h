/* 
 * File:   ConfigurationReader.h
 * Author: gonchub
 *
 * Created on March 21, 2013, 9:25 PM
 */

#ifndef CONFIGURATIONREADER_H
#define	CONFIGURATIONREADER_H

#include <model/entities/Entity.h>
#include <model/persistence/PersistentConfiguration.h>
#include <model/map/Tile.h>
#include <model/map/TextureHolder.h>
#include <view/configuration/AnimationConfiguration.h>

#include <string>

class ConfigurationReader {
public:
	ConfigurationReader();
	ConfigurationReader(const ConfigurationReader& orig);
	PersistentConfiguration loadConfiguration(std::string configurationFile);
	virtual ~ConfigurationReader();

private:
};

#endif	/* CONFIGURATIONREADER_H */

