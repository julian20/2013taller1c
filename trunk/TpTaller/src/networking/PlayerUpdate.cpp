/*
 * PlayerUpdate.cpp
 *
 *  Created on: 08/05/2013
 *      Author: tomas
 */

#include <networking/PlayerUpdate.h>

using namespace std;

PlayerUpdate::PlayerUpdate() :
		MobileEntityUpdate() {
	this->currentPos = new Vector3();
	this->endPos = new Vector3();
	this->speed = new Speed();
	this->attacking = false;
	this->blocking = false;
	this->active = true;
	this->currentTile = new Tile();
	this->nextTile = new Tile();
	this->initCoords = new Coordinates();
	this->chat = NULL;
}

void PlayerUpdate::setChat(Chat* chat) {
	this->chat = chat;
}

bool PlayerUpdate::isBlocking() {
	return blocking;
}

void PlayerUpdate::setBlocking(bool blocking) {
	this->blocking = blocking;
}

bool PlayerUpdate::isActive() {
	return active;
}

void PlayerUpdate::setActive(bool active) {
	this->active = active;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const PlayerUpdate& update) {

	out << update.name << " " << *update.currentPos << " " << *update.endPos
			<< " " << *update.speed << " " << update.attacking << " "
			<< update.blocking << " " << update.active << " "
			<< *update.currentTile << " " << *update.nextTile << " "
			<< *update.initCoords << " " << update.life << " " << update.magic
			<< " " << update.lastAttackingDirection << " " << update.team
			<< " " << update.castingSpell;

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, PlayerUpdate& update) {

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
	bool active;
	in >> active;
	update.setActive(active);
	Tile tile;
	in >> tile;
	update.setTile(&tile);
	in >> tile;
	update.setNextTile(&tile);
	Coordinates coords;
	in >> coords;
	update.setInitCoordinates(&coords);
	int life, magic;
	in >> life;
	update.setLife(life);
	in >> magic;
	update.setMagic(magic);
	in >> update.lastAttackingDirection;
	int team;
	in >> team;
	update.setTeam(team);
	int castingSp;
	in >> castingSp;
	update.setCastingSpell(castingSp);
	return in;

}

PlayerUpdate::~PlayerUpdate() {
}
