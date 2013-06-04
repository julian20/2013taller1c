/*
 * ArtificialIntelligence.cpp
 *
 *  Created on: 27/05/2013
 *      Author: damian
 */

#include <AI/ArtificialIntelligence.h>
#include <stdlib.h>
#define WATCHSIZE 10
ArtificialIntelligence::ArtificialIntelligence() {
	entity=NULL;
}
void ArtificialIntelligence::attack(Entity* enemy)
{

}
void ArtificialIntelligence::setEntity(Entity* entity)
{
	this->entity=entity;
}
void ArtificialIntelligence::update(MapData* mapData)
{
	if(this->isAnyEnemyClose(mapData))
	{
			Entity* enemy=this->getNearestEnemy();
			this->attack(enemy);
	}else
	{
		if(entity->getClassName() == "Mob")
		{
			this->watch();
			MobileEntity* mob= (MobileEntity*)entity;
			mob->update(mapData);
		}
	}

}
bool ArtificialIntelligence::isAnyEnemyClose(MapData* mapData)
{
	//mapData->getClosestEntities(entity->getCoordinates(),entity->get)
	return this->entity->getVisibleEnemies().size()>0;
}
Entity* ArtificialIntelligence::getNearestEnemy()
{
	list<Entity*> list=this->entity->getVisibleEnemies();
	Entity* enemy=list.front();
	return enemy;
}
void ArtificialIntelligence::watch()
{
	 MobileEntity* mobile=(MobileEntity*)(this->entity);
	 Coordinates coor=this->entity->getCoordinates();
	 //Si no se esta moviendo o atacando busco a otro contrincante
	 if(!mobile->IsMoving() || !mobile->isAttacking())
	 {
		 int direction = rand() % 4 ;
		 if(direction==0)
		 {
			 mobile->moveTo(coor.getCol(),coor.getRow()+WATCHSIZE);
		 }else if(direction==1)
		 {
			 mobile->moveTo(coor.getCol(),coor.getRow()-WATCHSIZE);
		 }else if(direction==2)
		 {
			 mobile->moveTo(coor.getCol()+WATCHSIZE,coor.getRow());
		 }else if(direction==3)
		 {
			 mobile->moveTo(coor.getCol()-WATCHSIZE,coor.getRow());
		 }
	 }
}
ArtificialIntelligence::~ArtificialIntelligence() {
}


