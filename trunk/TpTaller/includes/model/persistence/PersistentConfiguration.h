/*
 * PersistentConfiguration.h
 *
 *  Created on: Apr 1, 2013
 *      Author: gonchub
 */

#ifndef PERSISTENTCONFIGURATION_H_
#define PERSISTENTCONFIGURATION_H_

#include <model/entities/Entity.h>
#include <model/map/TextureHolder.h>

#include <vector>

class PersistentConfiguration {
public:
	PersistentConfiguration();
	virtual ~PersistentConfiguration();
	TextureHolder* getTextureHolder();
	void setTextureHolder(TextureHolder* textureHolder);
	std::vector<Entity*> getEntityList();
	void setEntityList(std::vector<Entity*> entityList);
private:
	std::vector<Entity*> entityList;
	TextureHolder* textureHolder;
};

#endif /* PERSISTENTCONFIGURATION_H_ */
