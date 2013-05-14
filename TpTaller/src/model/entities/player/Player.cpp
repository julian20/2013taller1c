/* 
 * File:   Personaje.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:13
 */

#include <model/entities/player/Player.h>
#include <model/entities/Entity.h>
#include <stdio.h>

Player::Player() {
	endPos = new Vector3(0, 0);
	this->mainPlayer = false;
	this->speed = new Speed(0, new Vector2(0, 0));
	this->initSpeed = NULL;
	this->name = "";
	this->path = new list<Tile *>();
	this->currentTile = new Tile(new Coordinates(0, 0));
	isActive = false;
	chat= new Chat();
	attacking = false;
	blocking = false;
	hasChange = true;
}
Chat* Player::getChat()
{
	return this->chat;
}
void Player::setPos(float x, float y, float z) {
	currentPos->setValues(x, y, z);

	endPos->setValues(currentPos->getX(), currentPos->getY());
}

void Player::moveTo(int x, int y, int z) {
	endPos = new Vector3(x, y, z);
}

bool Player::IsMoving() {
	return !(currentPos->isEqual(endPos));
}

bool Player::isRunning() {
	if (speed->getMagnitude() >= 2 * initSpeed->getMagnitude())
		return true;
	return false;
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

void Player::update(PlayerUpdate* update){

	this->currentPos->setValues(update->getCurrentPos()->getX(),update->getCurrentPos()->getY(),update->getCurrentPos()->getZ());
	this->endPos->setValues(update->getEndPos()->getX(),update->getEndPos()->getY(),update->getEndPos()->getZ());
	this->speed->setMagnitude(update->getSpeed()->getMagnitude());
	this->speed->setDirection(update->getSpeed()->getDirection());
	this->attacking = update->isAttacking();
	this->blocking = update->isBlocking();
	this->currentTile = update->getTile();

	Coordinates currentTileCoords = this->currentTile->getCoordinates();
	Coordinates nextTileCoords = update->getNextTile()->getCoordinates();
	if ( ( !currentTileCoords.isEqual( nextTileCoords ) ) && ( this->path->empty() ) ){
		this->path->push_front(update->getNextTile());
	}



}

PlayerUpdate* Player::generatePlayerUpdate(){

	if (!this->hasChange) return NULL;

	PlayerUpdate* update = new PlayerUpdate();

	update->setName(this->name);

	update->setCurrentPos(this->currentPos);
	update->setEndPos(this->endPos);
	update->setSpeed(this->speed);
	update->setAttacking(this->attacking);
	update->setBlocking(this->blocking);
	update->setTile(this->currentTile);
	update->setInitCoordinates(this->coord);
	if (!this->path->empty()){
		update->setNextTile(this->path->front());
	} else {
		update->setNextTile(this->currentTile);
	}

	return update;
}

void Player::loadNextPosition() {
	if (path->empty())
		return;
	Tile* tile = path->front();
	path->remove(tile);

	Position* tilePos = tile->getPosition();
	moveTo(tilePos->getX(), tilePos->getY());

	setTile(tile);
}

Vector2* Player::getMovementDirection() {
	Vector2* moveDirection = new Vector2(endPos->getX() - currentPos->getX(),
			endPos->getY() - currentPos->getY());
	Vector2* v = new Vector2(0, 0);

	if (moveDirection->isEqual(v)) {
		delete v;
		return moveDirection;
	}

	delete v;

	moveDirection->normalize();

	return moveDirection;
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

void Player::setPosition(Position* position) {
	Position* pos = Tile::computePosition(position->getX(), position->getY());
	currentPos->setValues(pos->getX(), pos->getY());
	delete pos;
}

Speed* Player::getSpeed() {
	return speed;
}

void Player::setSpeed(Speed* speed) {
	if (this->speed)
		delete this->speed;
	this->speed = speed;
	if (initSpeed == NULL)
		initSpeed = new Speed(speed->getMagnitude(), new Vector2(0, 0));
}

void Player::setInitSpeed(Speed* initSpeed) {
	this->initSpeed = initSpeed;
}

std::string Player::getName() {
	return name;
}

void Player::setName(std::string name) {
	this->name = name;
}

std::vector<Power*> Player::getPowers() {
	return powers;
}

void Player::setPowers(std::vector<Power*> powers) {
	this->powers = powers;
}

void Player::assignPath(list<Tile *> *_path) {
	if (path)
		delete path;
	this->path = _path;

	loadNextPosition();
}

void Player::setTile(Tile* _tile) {
	if (this->currentTile)
		delete this->currentTile;
	currentTile = _tile;

	// Las coordinates se actualizan en EntityViewMap
}

Tile* Player::getTile() {
	// Devuelve una copia del tile
	Tile* retval = new Tile();
	retval->setCoordinates(currentTile->getCoordinates());

	return retval;
}

string Player::getClassName() {
	return "Personaje";
}

void Player::setSpeedMagnitude(int mag) {
	speed->setMagnitude(mag);
}

bool Player::isAttacking() {
	return attacking;
}

void Player::attack() {
	attacking = true;
}

void Player::setAttack(bool attacking){
	this->attacking = attacking;
}

void Player::cancelAttack() {
	attacking = false;
	hasChange = true;
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
		hasChange = true;
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

void Player::emptyPath() {
	std::list<Tile *>::iterator iter;
	for (iter = path->begin(); iter != path->end(); ++iter) {
		Tile* tile = *iter;

		delete tile;
	}

	delete path;
	this->path = new list<Tile *>();
}

void Player::stop() {
	emptyPath();
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

Vector3* Player::getEndPos(){
	return endPos;
}

void Player::setEndPos(float x, float y, float z){
	endPos->setValues(x, y, z);
}

void Player::setChange(bool change){
	hasChange = change;
}

bool Player::getChange(){
	return hasChange;
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
