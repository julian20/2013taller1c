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
	this->speed = new Speed(0, Vector2(0, 0));
	this->initSpeed = NULL;
	this->name = "";
	this->path = new list<Tile *>();
	this->currentTile = new Tile( new Coordinates(0, 0) );
}

void Player::setPos(float x, float y, float z) {
	currentPos->setValues(x, y, z);

	endPos->setValues(currentPos->getX(), currentPos->getY());
}

void Player::moveTo(int x, int y, int z) {
	endPos = new Vector3(x, y, z);
}

bool Player::isRunning(){
	if (speed->getMagnitude() >= 2*initSpeed->getMagnitude())
		return true;
	return false;
}

void Player::update() {
	if (IsMoving() == false)
	{
		if (path->size() == 0) return;
		else loadNextPosition();
	}

	float relationSpeed = ((float) Tile::getTileHeight())/((float) Tile::getTileWidth());

	Vector3* moveDirection = new Vector3(endPos->getX() - currentPos->getX(),
			endPos->getY() - currentPos->getY(), endPos->getZ() - currentPos->getZ());

	if (moveDirection->getNorm() < getSpeed()->getMagnitude() + 1) {
		// Close enough to the end position to move in one step.
		currentPos->setValues(endPos->getX(), endPos->getY());
		if (path->size() == 0) return;
		else loadNextPosition();
	} else {
		moveDirection->normalize();
		moveDirection->multiplyBy(fabs(moveDirection->getY()) * (relationSpeed - 1) + 1);
		moveDirection->multiplyBy(getSpeed()->getMagnitude());
		currentPos->add(moveDirection);
	}
}

void Player::loadNextPosition() {
	Tile* tile = path->front();
	path->remove(tile);

	Position* tilePos = tile->getPosition();
	moveTo(tilePos->getX(), tilePos->getY());

	setTile(tile);
}

Vector2* Player::getMovementDirection() {
	Vector2* moveDirection = new Vector2(endPos->getX() - currentPos->getX(),
			endPos->getY() - currentPos->getY());
	moveDirection->normalize();

	return moveDirection;
}

Player::Player(string name, Position* position, Speed* speed, vector<Power*> powers){
	this->speed = speed;
	this->name = name;
	this->powers = powers;
	this->path = new list<Tile *>();
	currentTile = new Tile(new Coordinates(0, 0));
	this->currentPos = new Vector3(0, 0, 0);
	this->base = new Base();
	endPos = new Vector3(0, 0, 0);
	endPos->setValues(currentPos->getX(),currentPos->getY());
}

Player::~Player() {
	delete this->speed;
	if (initSpeed != NULL) delete initSpeed;
}

void Player::setPosition(Position* position) {
	Position pos = Tile::computePosition(position->getX(),position->getY());
	currentPos->setValues(pos.getX(), pos.getY());
	//currentPos->SetValues(Tile::computePosition(x,y).x , Tile::computePosition(x,y).y);
}

Speed* Player::getSpeed() {
	return speed;
}

void Player::setSpeed(Speed* speed) {
	this->speed = speed;
	if (initSpeed == NULL)
		initSpeed = new Speed(speed->getMagnitude(),Vector2(0,0));
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

bool Player::IsMoving() {
	return !(currentPos->isEqual(endPos));
}

void Player::assignPath(list<Tile *> *_path) {
	if (path) delete path;
	this->path = _path;

	loadNextPosition();
}

void Player::setTile( Tile* _tile ) {
	if (this->currentTile) delete this->currentTile;
	currentTile = _tile;

	// Las coordinates se actualizan en EntityViewMap
}

Tile Player::getTile() {
	Tile retval;
	retval.setCoordinates( currentTile->getCoordinates() );

	return retval;
}

string Player::getClassName(){
	return "Personaje";
}

void Player::setSpeedMagnitude(int mag){
	speed->setMagnitude(mag);
}
