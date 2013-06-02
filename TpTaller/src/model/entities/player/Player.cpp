/* 
 * File:   Personaje.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:13
 */

#include <model/entities/player/Player.h>
#include <model/entities/Entity.h>
#include <stdio.h>
using namespace std;

Player::Player() {
	endPos = new Vector3(0, 0);
	this->mainPlayer = false;
	this->speed = new Speed(0, new Vector2(0, 0));
	this->initSpeed = NULL;
	this->name = "";
	this->path = new list<Tile *>();
	this->currentTile = new Tile(new Coordinates(0, 0));
	this->weapons = new list<Weapon*>;
	this->team = 2;
	//TODO: sacar este harcode. tendria q venir de yaml
	Sword* sword = new Sword();
	sword->setAccuracy(10);
	sword->setDamage(10);
	sword->setRange(1);
	sword->setMagic(0);
	weapons->push_front(sword);
	isActive = true;
	attacking = false;
	blocking = false;
	hasChanged = true;
	chat = NULL;
	viewRange = 200;
}

void Player::setChat(Chat* chat) {
	if (this->chat) {
		delete this->chat;
	}
	this->chat = chat;
}

void Player::collideTo(Entity* entity) {
	//if(entity->getClassName()=="Item")
	attack(entity);
}

void Player::reverseCollide(Entity* entity) {

}

void Player::attack(Entity* entity) {
	if (attackTimer.getTimeIntervalSinceStart() > ATTACK_TIMEOUT) {
		attacking = true;
		//this->magic--;
		if(magic>0)
		{
			Weapon* weaponToUse = weapons->front();
			weaponToUse->attack(entity);
			this->magic-=weaponToUse->getMagic();
		}
	}

}

void Player::updateFromServer(PlayerUpdate* update) {

	this->currentPos->setValues(update->getCurrentPos()->getX(),
			update->getCurrentPos()->getY(), update->getCurrentPos()->getZ());
	this->endPos->setValues(update->getEndPos()->getX(),
			update->getEndPos()->getY(), update->getEndPos()->getZ());
	this->speed->setMagnitude(update->getSpeed()->getMagnitude());
	this->speed->setDirection(update->getSpeed()->getDirection());
	this->attacking = update->isAttacking();
	this->blocking = update->isBlocking();
	this->isActive = update->isActive();
	this->life = update->getLife();

	if (currentTile)
		delete currentTile;
	this->currentTile = update->getTile();

	Coordinates currentTileCoords = this->currentTile->getCoordinates();
	Coordinates nextTileCoords = update->getNextTile()->getCoordinates();
	if ((!currentTileCoords.isEqual(nextTileCoords)) && (this->path->empty())) {
		this->path->push_front(update->getNextTile());
	}

}
ChatUpdate* Player::generateChatUpdate() {
	if (!this->chat->hasChange()) {
		return NULL;
	} else {
		ChatUpdate* update = new ChatUpdate();

		update->setReceiver(this->chat->getReceptor());
		update->setMessage(this->chat->getMessageSend());
		update->setSender(this->name);

		return update;
	}
}

PlayerUpdate* Player::generatePlayerUpdate() {
	if (!this->hasChanged)
		return NULL;

	PlayerUpdate* update = new PlayerUpdate();

	update->setName(this->name);

	update->setCurrentPos(this->currentPos);
	update->setEndPos(this->endPos);
	update->setSpeed(this->speed);
	update->setAttacking(this->attacking);
	update->setBlocking(this->blocking);
	update->setActive(this->isActive);
	update->setTile(this->currentTile);
	update->setInitCoordinates(this->coord);
	update->setLife(this->life);
	update->setMagic(this->magic);
	if (!this->path->empty()) {
		update->setNextTile(this->path->front());
	} else {
		update->setNextTile(this->currentTile);
	}

	return update;
}

Player::Player(string name, Position* position, Speed* speed,
		vector<Power*> powers) {
	this->speed = speed;
	this->name = name;
	this->powers = powers;
	this->path = new list<Tile *>();
	currentTile = new Tile(new Coordinates(0, 0));
	this->currentPos = new Vector3(0, 0, 0);
	this->base = new Base();
	endPos = new Vector3(0, 0, 0);
	endPos->setValues(currentPos->getX(), currentPos->getY());
	attacking = false;
	blocking = false;
	chat = NULL;
	isActive = true;
	mainPlayer = true;
	viewRange = 200;

}

Player::~Player() {
	delete currentPos;
	delete endPos;
	delete this->speed;
	if (initSpeed != NULL)
		delete initSpeed;
	for (unsigned i = 0; i < powers.size(); i++) {
		delete powers[i];
	}
	if (currentTile)
		delete currentTile;
}

std::vector<Power*> Player::getPowers() {
	return powers;
}

void Player::setPowers(std::vector<Power*> powers) {
	this->powers = powers;
}

void Player::block() {
	blocking = true;
}

void Player::setBlock(bool blocking) {
	this->blocking = blocking;
}

void Player::cancelBlock() {
	if (blocking) {
		blocking = false;
		hasChanged = true;
	}
}

bool Player::isBlocking() {
	return blocking;
}

bool Player::isMainPlayer() {
	return mainPlayer;
}

void Player::setAsMainPlayer() {
	this->mainPlayer = true;
}

void Player::setViewRange(int _viewRange) {
	this->viewRange = _viewRange;
}

int Player::getViewRange() {
	return this->viewRange;
}

Player& Player::operator=(const Player &other) {

	this->name = other.name;
	*(this->currentPos) = *(other.currentPos);
	*(this->endPos) = *(other.endPos);
	*(this->speed) = *(other.speed);
	*(this->initSpeed) = *(other.initSpeed);
	*(this->base) = *(other.base);
	this->powers = other.powers;
	*(this->currentTile) = *(other.currentTile);
	*(this->path) = *(other.path);
	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Player& player) {
	out << player.name << " " << *(player.currentPos) << " " << *(player.endPos)
			<< " " << *(player.speed) << " " << *(player.initSpeed) << " "
			<< *(player.base) << " " << player.powers.size() << " ";
	for (unsigned int i = 0; i < player.powers.size(); i++) {
		out << *(player.powers[i]) << " ";
	}
	out << *(player.currentTile) << " ";
	out << " " << player.life << " " << player.team;
	out << " " << player.lastAttackingDirection;
	return out;
}

string Player::getClassName() {
	return "Player";
}
//Operator to load an object from a stream
istream& operator >>(std::istream& in, Player& player) {
	string name;
	in >> name;
	player.setName(name);
	Vector3 pos;
	in >> pos;
	player.setPos(pos.getX(), pos.getY(), pos.getZ());
	in >> pos;
	player.moveTo(pos.getX(), pos.getY(), pos.getZ());
	Speed* speed = new Speed();
	in >> *speed;
	player.setSpeed(speed);
	Speed* initSpeed = new Speed();
	in >> *initSpeed;
	player.setInitSpeed(initSpeed);
	Base* base = new Base();
	in >> *base;
	player.setBase(base);
	// Number of powers
	int n;
	in >> n;
	vector<Power*> powers;
	for (int i = 0; i < n; i++) {
		Power* power = new Power();
		in >> *power;
		powers.push_back(power);
	}
	player.setPowers(powers);
	Tile* tile = new Tile();
	in >> *tile;
	player.setTile(tile);
	int life;
	in >> life;
	player.life = life;
	int team;
	in >> team;
	player.team = team;
	float lastDir;
	in >> lastDir;
	player.lastAttackingDirection = lastDir;
	return in;
}

void Player::setActive() {
	isActive = true;
}

void Player::SetUnactive() {
	isActive = false;
}

bool Player::playerIsActive() {
	return isActive;
}
