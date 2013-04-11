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
	endPos = new Vector2(0, 0);
	this->position = new Position(0, 0, 0);
	this->speed = new Speed(0, Vector2(0, 0));
	this->name = "";
	this->path = new list<Tile *>();
	this->currentTile = new Tile( new Coordinates(0, 0) );
}

void Player::setPos(float x, float y) {
	currentPos->setValues(x,y);

	endPos->setValues(currentPos->getX(), currentPos->getY());
}

void Player::MoveTo(int x, int y) {
	endPos = new Vector2(x, y);
}


bool Player::isRunning(){
	if (speed->getMagnitude() >= 5)
		return true;
	return false;
}

void Player::update() {
	if (IsMoving() == false)
	{
		if (path->size() == 0) return;
		else setNextPosition();
	}

	Vector2* moveDirection = new Vector2(endPos->getX() - currentPos->getX(),
			endPos->getY() - currentPos->getY());

	if (moveDirection->getNorm() < getSpeed()->getMagnitude() + 1) {
		// Close enough to the end position to move in one step.
		currentPos->setValues(endPos->getX(), endPos->getY());
	} else {
		moveDirection->normalize();
		moveDirection->multiplyBy(getSpeed()->getMagnitude());
		currentPos->add(moveDirection);
		if (path->size() == 0) return;
		else setNextPosition();
	}
}

void Player::setNextPosition() {
	Tile* tile = path->front();
	path->remove(tile);

	Position* tilePos = tile->getPosition();
	MoveTo(tilePos->getX(), tilePos->getY());

	delete currentTile;
	currentTile = tile;
}

Vector2* Player::GetCurrentPos() {
	return new Vector2(currentPos->getX(), currentPos->getY());
}

Vector2* Player::GetMovementDirection() {
	Vector2* moveDirection = new Vector2(endPos->getX() - currentPos->getX(),
			endPos->getY() - currentPos->getY());
	moveDirection->normalize();

	return moveDirection;
}

Player::Player(string name, Position* position, Speed* speed, vector<Power*> powers){
	this->position = position;
	this->speed = speed;
	this->name = name;
	this->powers = powers;
	this->path = new list<Tile *>();
	currentTile = new Tile(new Coordinates(0, 0));
	this->currentPos = new Vector2(0, 0);
	this->base = new Base();
	endPos = new Vector2(0,0);
	endPos->setValues(currentPos->getX(),currentPos->getY());
}

Player::~Player() {
	delete this->position;
	delete this->speed;
}

Position* Player::getPosition() {
	return position;
}

void Player::setPosition(Position* position) {
	this->position = position;
	int x = position->getX();
	int y = position->getY();
	//currentPos->SetValues(Tile::computePosition(x,y).x , Tile::computePosition(x,y).y);
}

Speed* Player::getSpeed() {
	return speed;
}

void Player::setSpeed(Speed* speed) {
	this->speed = speed;
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
}

void Player::setTile( Tile* _tile ) {

	if (this->currentTile) delete this->currentTile;
	currentTile = _tile;
}

Tile* Player::getTile() {
	return currentTile;
}


string Player::getClassName(){
	return "Personaje";
}

void Player::setSpeedMagnitude(int mag){
	speed->setMagnitude(mag);
}
