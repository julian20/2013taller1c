/*
 * MobileEntityUpdate.cpp
 *
 *  Created on: 08/05/2013
 *      Author: tomas
 */

#include <networking/MobileEntityUpdate.h>
using namespace std;

MobileEntityUpdate::MobileEntityUpdate() {
	this->id = 0;
	this->currentPos = new Vector3();
	this->endPos = new Vector3();
	this->speed = new Speed();
	this->attacking = false;
	this->currentTile = new Tile();
	this->nextTile = new Tile();
	this->initCoords = new Coordinates;
	this->life = 0;
	this->magic = 0;
	this->lastAttackingDirection = 0;
	this->team = 0;
	this->castingSpell = false;
	this->viewRange = 200;
	this->frozen = false;
}

void MobileEntityUpdate::setId(int id) {
	this->id = id;
}

int MobileEntityUpdate::getId() {
	return this->id;
}

void MobileEntityUpdate::setName(string MobileEntityName) {
	this->name = MobileEntityName;
}

string MobileEntityUpdate::getName() {
	return this->name;
}

Vector3* MobileEntityUpdate::getCurrentPos() {
	return currentPos;
}

void MobileEntityUpdate::setLife(int life) {
	this->life = life;
}
void MobileEntityUpdate::setMagic(int magic) {
	this->magic = magic;
}

int MobileEntityUpdate::getLife() {
	return life;
}
int MobileEntityUpdate::getMagic() {
	return magic;
}

void MobileEntityUpdate::setTeam(int team) {
	this->team = team;
}

int MobileEntityUpdate::getTeam() {
	return team;
}

void MobileEntityUpdate::setCastingSpell(bool castingSpell) {
	this->castingSpell = castingSpell;
}

bool MobileEntityUpdate::getCastingSpell() {
	return castingSpell;
}

void MobileEntityUpdate::setViewRange(int viewRange) {
	this->viewRange = viewRange;
}

void MobileEntityUpdate::setFrozen(bool frozen) {
	this->frozen = frozen;
}

bool MobileEntityUpdate::getFrozen() {
	return frozen;
}

int MobileEntityUpdate::getViewRange() {
	return viewRange;
}

void MobileEntityUpdate::setCurrentPos(Vector3* currentPos) {
	this->currentPos->setValues(currentPos->getX(), currentPos->getY(),
			currentPos->getZ());
}

Vector3* MobileEntityUpdate::getEndPos() {
	return endPos;
}

void MobileEntityUpdate::setEndPos(Vector3* endPos) {
	this->endPos->setValues(endPos->getX(), endPos->getY(), endPos->getZ());
}

Speed* MobileEntityUpdate::getSpeed() {
	return speed;
}

void MobileEntityUpdate::setSpeed(Speed* speed) {
	this->speed->setMagnitude(speed->getMagnitude());
	this->speed->setDirection(speed->getDirection());
}

bool MobileEntityUpdate::isAttacking() {
	return attacking;
}

void MobileEntityUpdate::setAttacking(bool attacking) {
	this->attacking = attacking;
}

float MobileEntityUpdate::getLastAttackingDirection() {
	return this->lastAttackingDirection;
}

void MobileEntityUpdate::setLastAttackingDirection(float dir) {
	this->lastAttackingDirection = dir;
}

Tile* MobileEntityUpdate::getTile() {
	Tile* tile = new Tile(
			new Coordinates(currentTile->getCoordinates().getRow(),
					currentTile->getCoordinates().getCol()));
	return tile;
}

void MobileEntityUpdate::setTile(Tile* tile) {
	this->currentTile->setFScore(tile->getFScore());
	this->currentTile->setPosition(tile->getPosition());
	this->currentTile->setCoordinates(tile->getCoordinates());
}

void MobileEntityUpdate::setNextTile(Tile* nextTile) {
	this->nextTile->setFScore(nextTile->getFScore());
	this->nextTile->setPosition(nextTile->getPosition());
	this->nextTile->setCoordinates(nextTile->getCoordinates());
}

Tile* MobileEntityUpdate::getNextTile() {
	Tile* tile = new Tile(
			new Coordinates(nextTile->getCoordinates().getRow(),
					nextTile->getCoordinates().getCol()));
	return tile;
}

MobileEntityUpdate::~MobileEntityUpdate() {
	delete currentPos;
	delete endPos;
	delete speed;
	delete currentTile;
	delete nextTile;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const MobileEntityUpdate& update) {

	out << update.id << " " << update.name << " " << *update.currentPos << " "
			<< *update.endPos << " " << *update.speed << " " << update.attacking
			<< " " << " " << *update.currentTile << " " << *update.nextTile
			<< " " << update.life << " "
			<< update.magic << " " << update.lastAttackingDirection << " "
			<< update.team << " " << update.viewRange << " " << update.frozen;

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, MobileEntityUpdate& update) {
	in >> update.id;
	in >> update.name;
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
	Tile tile;
	in >> tile;
	update.setTile(&tile);
	in >> tile;
	update.setNextTile(&tile);
	int life, magic;
	in >> life;
	update.setLife(life);
	in >> magic;
	update.setMagic(magic);
	in >> update.lastAttackingDirection;
	int team;
	in >> team;
	update.setTeam(team);
	int view;
	in >> view;
	update.setViewRange(view);
	bool frozen;
	in >> frozen;
	update.frozen = frozen;
	return in;
}
