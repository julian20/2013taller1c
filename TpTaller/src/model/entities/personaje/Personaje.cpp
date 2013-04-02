/* 
 * File:   Personaje.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:13
 */

#include <model/entities/personaje/Personaje.h>
#include <stdio.h>
Personaje::Personaje() {
    endPos = new Vector2(0, 0);
    velocity = 3;
}

void Personaje::setPos(float x, float y) {
	printf("personaje\n");
	 //Llamamos al super
	 Entity::setPos(x,y);

	 endPos->SetValues( currentPos->GetX(), currentPos->GetY() );
}

void Personaje::MoveTo(int x, int y) {
	endPos = new Vector2(x , y);
}

void Personaje::Update() {
	if (IsMoving() == false) return;

	Vector2* moveDirection = new Vector2(endPos->GetX() - currentPos->GetX() ,
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
	return new Vector2( currentPos->GetX(), currentPos->GetY());
}

Vector2* Personaje::GetMovementDirection() {
	Vector2* moveDirection = new Vector2(endPos->GetX() - currentPos->GetX() ,
										 endPos->GetY() - currentPos->GetY());
	moveDirection->Normalize();

	return moveDirection;
}

bool Personaje::IsMoving() {
	return !(currentPos->IsEqual(endPos));
}

Personaje::~Personaje() {
}

