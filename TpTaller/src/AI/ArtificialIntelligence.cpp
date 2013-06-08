/*
 * ArtificialIntelligence.cpp
 *
 *  Created on: 27/05/2013
 *      Author: damian
 */

#include <AI/ArtificialIntelligence.h>
#include <stdlib.h>
#define WATCHSIZE 5
ArtificialIntelligence::ArtificialIntelligence() {
	entity=NULL;
}
void ArtificialIntelligence::attack(Entity& enemy)
{
		Mob* toAttack=(Mob*)entity;
		toAttack->attackTo(&enemy);
		//toAttack->attack(enemy);
}
void ArtificialIntelligence::setEntity(Entity* entity)
{
	this->entity=entity;
}
void ArtificialIntelligence::update(MapData* mapData)
{
	if(this->isAnyEnemyClose(mapData))
	{
			Entity& enemy=this->getNearestEnemy();
			this->attack(enemy);
			MobileEntity* mob=(MobileEntity*)this->entity;
			mob->update(mapData);

	}else
	{
		//entity->
		if(entity->getClassName() == "MobileEntity")
		{


			MobileEntity* mob= (MobileEntity*)entity;
			mob->cancelAttack();
			this->watch(mapData);
			mob->update(mapData);
		}
	}

}
bool ArtificialIntelligence::isAnyEnemyClose(MapData* mapData)
{
	entitiesNear=mapData->getClosestEntities(entity->getCoordinates(),WATCHSIZE);
	for ( list<MobileEntity*>::iterator mobIterator = entitiesNear.begin() ; mobIterator != entitiesNear.end() ; ++mobIterator ){
		MobileEntity* mob = *mobIterator;
		if(mob->getTeam()!=this->entity->getTeam())
		{
			return true;
		}
	}
	return false;
	//return false; // TODO :cambiar este harcoding
}
Entity& ArtificialIntelligence::getNearestEnemy()
{

	MobileEntity* mob;
	//Entity* enemy=entitiesNear.front();
	for ( list<MobileEntity*>::iterator mobIterator = entitiesNear.begin() ; mobIterator != entitiesNear.end() ; ++mobIterator ){
			mob= *mobIterator;
			if(mob->getTeam()!=this->entity->getTeam())
			{
				return *mob;
			}
		}
	return *mob;
	//return (*enemy);
}
void ArtificialIntelligence::watch(MapData* mapData)
{
	 MobileEntity* mobile=(MobileEntity*)(this->entity);
	 Coordinates coor=this->entity->getCoordinates();

	 Tile* tile;
	 //Si no se esta moviendo o atacando busco a otro contrincante
	 if(!mobile->IsMoving() )
	 {
		 int direction = rand() % 4 ;
		 if(direction==0)
		 {
			 //mobile->moveTo(coor.getCol(),coor.getRow()+WATCHSIZE);
			 tile =  new Tile( new Coordinates(coor.getRow(),coor.getCol()+WATCHSIZE));

		 }else if(direction==1)
		 {
			 //mobile->moveTo(coor.getCol(),coor.getRow()-WATCHSIZE);
			 tile =  new Tile( new Coordinates(coor.getRow(),coor.getCol()-WATCHSIZE));
		 }else if(direction==2)
		 {
			 //mobile->moveTo(coor.getCol()+WATCHSIZE,coor.getRow());
			 tile =  new Tile( new Coordinates(coor.getRow()+WATCHSIZE,coor.getCol()));
		 }else if(direction==3)
		 {
			 //mobile->moveTo(coor.getCol()-WATCHSIZE,coor.getRow());
			 tile =  new Tile( new Coordinates(coor.getRow()-WATCHSIZE,coor.getCol()));
		 }
		 if (tile->getCoordinates().getRow() > 0 && tile->getCoordinates().getRow() < mapData->getNRows()){
			 if (tile->getCoordinates().getCol() > 0 && tile->getCoordinates().getCol() < mapData->getNCols()){
				 mapData->moveMobileEntity((Mob*)this->entity,tile);
			 }
		 }
		 if (tile) delete tile;
	 }
}
ArtificialIntelligence::~ArtificialIntelligence() {
}


