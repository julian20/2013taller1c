/*
 * Vector.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: fede
 */

#include <model/Vector2.h>
#include <stdio.h>
#define TOLERANCE 0.1

Vector2::Vector2(float _x, float _y) {
	setValues(_x, _y);
}

float Vector2::getX() {
	return x;
}

float Vector2::getY() {
	return y;
}

void Vector2::setValues(float _x, float _y) {
	x = _x;
	y = _y;
}

void Vector2::normalize() {
	float norm = getNorm();

	x /= norm;
	y /= norm;
}

float Vector2::getNorm() {
	// sqrt( x² + y² );
	return (float) sqrt(pow((double) x, 2) + pow((double) y, 2));
}

void Vector2::multiplyBy(float val) {
	x *= val;
	y *= val;
}

void Vector2::add(Vector2 *other) {
	x += other->getX();
	y += other->getY();
}

void Vector2::sub(Vector2 *other) {
	x -= other->getX();
	y -= other->getY();
}

bool Vector2::isEqual(Vector2 *other) {
	bool XEquality = (x >= other->getX() - TOLERANCE
			&& x <= other->getX() + TOLERANCE);
	bool YEquality = (y >= other->getY() - TOLERANCE
			&& y <= other->getY() + TOLERANCE);

	return (XEquality && YEquality);
}

float Vector2::getAngle() {
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

Vector2& Vector2::operator=(Vector2 &other){
	this->x = other.x;
	this->y = other.y;
	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Vector2& vect){
	out << vect.x << " " << vect.y;
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Vector2& vect){
	float x,y;
	in >> x;
	in >> y;
	vect.setValues(x,y);
	return in;
}
