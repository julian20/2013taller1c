/*
 * MobileEntityUpdate.cpp
 *
 *  Created on: 08/05/2013
 *      Author: tomas
 */

#include <networking/MobileEntityUpdate.h>
using namespace std;

MobileEntityUpdate::MobileEntityUpdate(){
	this->currentPos = new Vector3();
	this->endPos = new Vector3();
	this->speed = new Speed();
	this->attacking = false;
	this->currentTile = new Tile();
	this->nextTile = new Tile();
	this->initCoords = new Coordinates;
}

void MobileEntityUpdate::setName(string MobileEntityName){
	this->name = MobileEntityName;
}

string MobileEntityUpdate::getName(){
	return this->name;
}

Vector3* MobileEntityUpdate::getCurrentPos(){
	return currentPos;
}

void MobileEntityUpdate::setCurrentPos(Vector3* currentPos){
	this->currentPos->setValues(currentPos->getX(),currentPos->getY(), currentPos->getZ());
}

Vector3* MobileEntityUpdate::getEndPos(){
	return endPos;
}

void MobileEntityUpdate::setEndPos(Vector3* endPos){
	this->endPos->setValues(endPos->getX(),endPos->getY(), endPos->getZ());
}

Speed* MobileEntityUpdate::getSpeed(){
	return speed;
}

void MobileEntityUpdate::setSpeed(Speed* speed){
	this->speed->setMagnitude(speed->getMagnitude());
	this->speed->setDirection(speed->getDirection());
}

bool MobileEntityUpdate::isAttacking(){
	return attacking;
}

void MobileEntityUpdate::setAttacking(bool attacking){
	this->attacking = attacking;
}


Tile* MobileEntityUpdate::getTile(){
	return currentTile;
}

void MobileEntityUpdate::setTile(Tile* tile){
	this->currentTile->setFScore(tile->getFScore());
	this->currentTile->setPosition(tile->getPosition());
	this->currentTile->setCoordinates(tile->getCoordinates());
}

Coordinates* MobileEntityUpdate::getInitCoordinates(){
	return initCoords;
}

void MobileEntityUpdate::setInitCoordinates(Coordinates* initCoords){
	this->initCoords->setCol(initCoords->getCol());
	this->initCoords->setRow(initCoords->getRow());
}

void MobileEntityUpdate::setNextTile(Tile* nextTile){
	this->nextTile->setFScore(nextTile->getFScore());
	this->nextTile->setPosition(nextTile->getPosition());
	this->nextTile->setCoordinates(nextTile->getCoordinates());
}

Tile* MobileEntityUpdate::getNextTile(){
	return nextTile;
}


MobileEntityUpdate::~MobileEntityUpdate() {
	// TODO Auto-generated destructor stub
}
