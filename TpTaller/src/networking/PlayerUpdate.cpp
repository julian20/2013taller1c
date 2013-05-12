/*
 * PlayerUpdate.cpp
 *
 *  Created on: 08/05/2013
 *      Author: tomas
 */

#include <networking/PlayerUpdate.h>

namespace std {

PlayerUpdate::PlayerUpdate(){
	this->currentPos = new Vector3();
	this->endPos = new Vector3();
	this->speed = new Speed();
	this->attacking = false;
	this->blocking = false;
	this->currentTile = new Tile();
	this->initCoords = new Coordinates;
}

void PlayerUpdate::setName(string playerName){
	this->name = playerName;
}
string PlayerUpdate::getName(){
	return this->name;
}

Vector3* PlayerUpdate::getCurrentPos(){
	return currentPos;
}

void PlayerUpdate::setCurrentPos(Vector3* currentPos){
	this->currentPos->setValues(currentPos->getX(),currentPos->getY(), currentPos->getZ());
}

Vector3* PlayerUpdate::getEndPos(){
	return endPos;
}

void PlayerUpdate::setEndPos(Vector3* endPos){
	this->endPos->setValues(endPos->getX(),endPos->getY(), endPos->getZ());
}

Speed* PlayerUpdate::getSpeed(){
	return speed;
}

void PlayerUpdate::setSpeed(Speed* speed){
	this->speed->setMagnitude(speed->getMagnitude());
	this->speed->setDirection(speed->getDirection());
}

bool PlayerUpdate::isAttacking(){
	return attacking;
}

void PlayerUpdate::setAttacking(bool attacking){
	this->attacking = attacking;
}

bool PlayerUpdate::isBlocking(){
	return blocking;
}

void PlayerUpdate::setBlocking(bool blocking){
	this->blocking = blocking;
}

Tile* PlayerUpdate::getTile(){
	return currentTile;
}

void PlayerUpdate::setTile(Tile* tile){
	this->currentTile->setFScore(tile->getFScore());
	this->currentTile->setPosition(tile->getPosition());
	this->currentTile->setCoordinates(tile->getCoordinates());
}

Coordinates* PlayerUpdate::getInitCoordinates(){
	return initCoords;
}

void PlayerUpdate::setInitCoordinates(Coordinates* initCoords){
	this->initCoords->setCol(initCoords->getCol());
	this->initCoords->setRow(initCoords->getRow());
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const PlayerUpdate& update){

	out << update.name << " " << *update.currentPos << " " << *update.endPos << " " << *update.speed << " " << update.attacking << " "
			<< update.blocking << " " << *update.currentTile << " " << *update.initCoords;

	return out;
}

	//Operator to load an object from a stream
istream& operator >>(std::istream& in, PlayerUpdate& update){

	string name;
	in >> name;
	update.setName(name);
	Vector3 vect;
	// 1ro currentPos
	in >> vect;
	update.setCurrentPos(&vect);
	// luego endPos
	in >> vect;
	update.setEndPos(&vect);
	Speed speed;
	in >> speed;
	update.setSpeed(&speed);
	bool attacking;
	in >> attacking;
	update.setAttacking(attacking);
	bool blocking;
	in >> blocking;
	update.setBlocking(blocking);
	Tile tile;
	in >> tile;
	update.setTile(&tile);
	Coordinates coords;
	in >> coords;
	update.setInitCoordinates(&coords);

	return in;
}


PlayerUpdate::~PlayerUpdate() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
