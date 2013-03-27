/*
 * Vector.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: fede
 */

#include <model/Vector2.h>

#define TOLERANCE 0.1

Vector2::Vector2(float _x, float _y){
	SetValues(_x, _y);
}

float Vector2::GetX() {
	return x;
}

float Vector2::GetY() {
	return y;
}

void Vector2::SetValues(float _x, float _y) {
	x = _x;
	y = _y;
}

void Vector2::Normalize() {
	float norm = GetNorm();

	x /= norm;
	y /= norm;
}

float Vector2::GetNorm() {
	// sqrt( x² + y² );
	return (float)sqrt( pow((double) x, 2) + pow((double) y, 2) );
}

void Vector2::MultiplyBy(float val) {
	x *= val;
	y *= val;
}

void Vector2::Add(Vector2 *other){
	x += other->GetX();
	y += other->GetY();
}

void Vector2::Sub(Vector2 *other){
	x -= other->GetX();
	y -= other->GetY();
}

bool Vector2::IsEqual(Vector2 *other) {
	bool XEquality = (x >= other->GetX() - TOLERANCE && x <= other->GetX() + TOLERANCE);
	bool YEquality = (y >= other->GetY() - TOLERANCE && y <= other->GetX() + TOLERANCE);

	return (XEquality && YEquality);
}
