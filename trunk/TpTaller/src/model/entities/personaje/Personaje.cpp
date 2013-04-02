/* 
 * File:   Personaje.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:13
 */

#include <model/entities/personaje/Personaje.h>
#include <model/entities/Entity.h>
#include <stdio.h>

Personaje::Personaje() {
	endPos = new Vector2(0, 0);
	velocity = 3;
	this->position = new Position(0, 0, 0);
	this->speed = new Speed(0, new Position(0, 0, 0));
	this->name = "";
}

void Personaje::setPos(float x, float y) {
	printf("personaje\n");
	//Llamamos al super
	Entity::setPos(x, y);

	endPos->SetValues(currentPos->GetX(), currentPos->GetY());
}

void Personaje::MoveTo(int x, int y) {
	endPos = new Vector2(x, y);
}

void Personaje::Update() {
	if (IsMoving() == false)
		return;

	Vector2* moveDirection = new Vector2(endPos->GetX() - currentPos->GetX(),
			endPos->GetY() - currentPos->GetY());

	if (moveDirection->GetNorm() < velocity + 1) {
		// Close enough to the end position to move in one step.
		currentPos->SetValues(endPos->GetX(), endPos->GetY());
	} else {
		moveDirection->Normalize();
		moveDirection->MultiplyBy(velocity);

		currentPos->Add(moveDirection);
	}
}

Vector2* Personaje::GetCurrentPos() {
	return new Vector2(currentPos->GetX(), currentPos->GetY());
}

Vector2* Personaje::GetMovementDirection() {
	Vector2* moveDirection = new Vector2(endPos->GetX() - currentPos->GetX(),
			endPos->GetY() - currentPos->GetY());
	moveDirection->Normalize();

	return moveDirection;
}

Personaje::Personaje(std::string name, Position* position, Speed* speed,
		std::vector<Power*> powers) {
	this->position = position;
	this->speed = speed;
	this->name = name;
	this->powers = powers;
	endPos = new Vector2(0, 0);
	velocity = 3;
}

Personaje::~Personaje() {
	delete this->position;
	delete this->speed;
}

const Position* Personaje::getPosition() const {
	return position;
}

void Personaje::setPosition(Position* position) {
	this->position = position;
}

const Speed* Personaje::getSpeed() const {
	return speed;
}

void Personaje::setSpeed(Speed* speed) {
	this->speed = speed;
}

const std::string Personaje::getName() const {
	return name;
}

void Personaje::setName(std::string name) {
	this->name = name;
}

std::vector<Power*> Personaje::getPowers() {
	return powers;
}

void Personaje::setPowers(const std::vector<Power*> powers) {
	this->powers = powers;
}

bool Personaje::IsMoving() {
	return !(currentPos->IsEqual(endPos));
}

