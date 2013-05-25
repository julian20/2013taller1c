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
	isActive = true;
	attacking = false;
	blocking = false;
	hasChanged = true;
}

void Player::update() {
	if (IsMoving() == false) {
		if (path->size() == 0)
			return;
		else
			loadNextPosition();
	}

	float relationSpeed = ((float) Tile::getTileHeight())
			/ ((float) Tile::getTileWidth());

	Vector3* moveDirection = new Vector3(endPos->getX() - currentPos->getX(),
			endPos->getY() - currentPos->getY(),
			endPos->getZ() - currentPos->getZ());

	if (moveDirection->getNorm() < getSpeed()->getMagnitude() + 1) {
		// Close enough to the end position to move in one step.
		currentPos->setValues(endPos->getX(), endPos->getY());
		if (path->size() == 0)
			return;
		else
			loadNextPosition();
	} else {
		moveDirection->normalize();
		moveDirection->multiplyBy(
				fabs(moveDirection->getY()) * (relationSpeed - 1) + 1);
		moveDirection->multiplyBy(getSpeed()->getMagnitude());
		currentPos->add(moveDirection);
	}
}

void Player::setChat(Chat* chat)
{
	if(this->chat)
	{
		delete this->chat;
	}
	this->chat=chat;
}

void Player::update(PlayerUpdate* update){

	this->currentPos->setValues(update->getCurrentPos()->getX(),update->getCurrentPos()->getY(),update->getCurrentPos()->getZ());
	this->endPos->setValues(update->getEndPos()->getX(),update->getEndPos()->getY(),update->getEndPos()->getZ());
	this->speed->setMagnitude(update->getSpeed()->getMagnitude());
	this->speed->setDirection(update->getSpeed()->getDirection());
	this->attacking = update->isAttacking();
	this->blocking = update->isBlocking();
	this->isActive = update->isActive();
	this->currentTile = update->getTile();

	Coordinates currentTileCoords = this->currentTile->getCoordinates();
	Coordinates nextTileCoords = update->getNextTile()->getCoordinates();
	if ( ( !currentTileCoords.isEqual( nextTileCoords ) ) && ( this->path->empty() ) ){
		this->path->push_front(update->getNextTile());
	}

}
ChatUpdate* Player::generateChatUpdate()
{
	//cout<<this->chat->getMessageSend()<<endl;
	if(!this->chat->hasChange())
		{
			return NULL;
		}
	else
	{
		cout<<"cambio el chat"<<endl;
		ChatUpdate* update =  new ChatUpdate();

		update->setReceiver(this->chat->getReceptor());
		update->setMessage(this->chat->getMessageSend());
		update->setSender(this->name);

		return update;
	}
}

PlayerUpdate* Player::generatePlayerUpdate(){
	if (!this->hasChanged) return NULL;

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
}

Player::~Player() {
	delete currentPos;
	delete endPos;
	delete this->speed;
	if (initSpeed != NULL)
		delete initSpeed;
	for (int i = 0; i < powers.size(); i++) {
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

void Player::setBlock(bool blocking){
	this->blocking = blocking;
}

void Player::cancelBlock() {
	if (blocking){
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
	return in;
}

void Player::setChange(bool change){
	hasChanged = change;
}

bool Player::getChange(){
	return hasChanged;
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
