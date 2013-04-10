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
	currentPos->SetValues(x,y);

	endPos->SetValues(currentPos->GetX(), currentPos->GetY());
}

void Player::MoveTo(int x, int y) {
	endPos = new Vector2(x, y);
}


bool Player::isRunning(){
	if (speed->getMagnitude() >= 5)
		return true;
	return false;
}

void Player::Update() {
	if (IsMoving() == false)
	{
		if (path->size() == 0) return;
		else setNextPosition();
	}

	Vector2* moveDirection = new Vector2(endPos->GetX() - currentPos->GetX(),
			endPos->GetY() - currentPos->GetY());

	if (moveDirection->GetNorm() < getSpeed()->getMagnitude() + 1) {
		// Close enough to the end position to move in one step.
		currentPos->SetValues(endPos->GetX(), endPos->GetY());
	} else {
		moveDirection->Normalize();
		moveDirection->MultiplyBy(getSpeed()->getMagnitude());
		currentPos->Add(moveDirection);
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
	return new Vector2(currentPos->GetX(), currentPos->GetY());
}

Vector2* Player::GetMovementDirection() {
	Vector2* moveDirection = new Vector2(endPos->GetX() - currentPos->GetX(),
			endPos->GetY() - currentPos->GetY());
	moveDirection->Normalize();

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
	endPos->SetValues(currentPos->GetX(),currentPos->GetY());
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
	return !(currentPos->IsEqual(endPos));
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
