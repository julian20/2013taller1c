/*
 * Vector.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: fede
 */

#include <model/Vector3.h>
#include <stdio.h>
#define TOLERANCE 0.1

Vector3::Vector3(float _x, float _y, float _z) {
	SetValues(_x, _y, _z);
}

float Vector3::GetX() {
	return x;
}

float Vector3::GetY() {
	return y;
}

float Vector3::GetZ() {
	return z;
}

void Vector3::SetValues(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

void Vector3::Normalize() {
	float norm = GetNorm();

	x /= norm;
	y /= norm;
	z /= norm;
}

float Vector3::GetNorm() {
	// sqrt( x² + y² + z² );
	return (float) sqrt(pow((double) x, 2) + pow((double) y, 2) + pow((double) z, 2));
}

void Vector3::MultiplyBy(float val) {
	x *= val;
	y *= val;
	z *= val;
}

void Vector3::Add(Vector3 *other) {
	x += other->GetX();
	y += other->GetY();
	z += other->GetZ();
}

void Vector3::Sub(Vector3 *other) {
	x -= other->GetX();
	y -= other->GetY();
	z -= other->GetZ();
}

bool Vector3::IsEqual(Vector3 *other) {
	bool XEquality = (x >= other->GetX() - TOLERANCE
			&& x <= other->GetX() + TOLERANCE);
	bool YEquality = (y >= other->GetY() - TOLERANCE
			&& y <= other->GetY() + TOLERANCE);
	bool ZEquality = (z >= other->GetZ() - TOLERANCE
			&& z <= other->GetZ() + TOLERANCE);

	return (XEquality && YEquality && ZEquality);
}

float Vector3::GetAngle() {
	if (x > 0 && y >= 0) {
		return atan(y / x);
	} else if (x < 0) {
		return atan(y / x) + M_PI;
	} else if (x > 0 && y < 0) {
		return atan(y / x) + 2 * M_PI;
	} else // this.x == 0
	{
		if (y > 0)
			return M_PI / 2;
		return M_PI * 3 / 2;
	}
}
