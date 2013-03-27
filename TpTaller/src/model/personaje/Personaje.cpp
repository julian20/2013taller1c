/* 
 * File:   Personaje.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:13
 */

#include <model/Personaje.h>

#define OFFSET_X	26
#define OFFSET_Y	0

Personaje::Personaje() {
    currentPos = new Vector2(OFFSET_X, OFFSET_Y);
    endPos = new Vector2(OFFSET_X, OFFSET_Y);
    velocity = 3;
}

void Personaje::SetPos(float x, float y) {
	 currentPos->SetValues(x + OFFSET_X, y + OFFSET_Y);
	 endPos->SetValues( currentPos->GetX(), currentPos->GetY() );
}

void Personaje::MoveTo(int x, int y) {
	endPos = new Vector2(x + OFFSET_X , y + OFFSET_Y);
}

void Personaje::Update() {
	if (currentPos->IsEqual(endPos)) return;	// Si ya llego a la posicion final

	Vector2* moveDirection = new Vector2(endPos->GetX() - currentPos->GetX() ,
										 endPos->GetY() - currentPos->GetY());

	if (moveDirection->GetNorm() < velocity) {
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

bool IRepresentable::Transitable() {
	return true;
}

IRepresentable::~IRepresentable() {
}

Personaje::~Personaje() {
}

