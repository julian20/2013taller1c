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
	setValues(_x, _y, _z);
}

float Vector3::getX() {
	return x;
}

float Vector3::getY() {
	return y;
}

float Vector3::getZ() {
	return z;
}

void Vector3::setValues(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

void Vector3::normalize() {
	float norm = getNorm();

	x /= norm;
	y /= norm;
	z /= norm;
}

float Vector3::getNorm() {
	// sqrt( x² + y² + z² );
	return (float) sqrt(
			pow((double) x, 2) + pow((double) y, 2) + pow((double) z, 2));
}

void Vector3::multiplyBy(float val) {
	x *= val;
	y *= val;
	z *= val;
}

void Vector3::add(Vector3 *other) {
	x += other->getX();
	y += other->getY();
	z += other->getZ();
}

void Vector3::sub(Vector3 *other) {
	x -= other->getX();
	y -= other->getY();
	z -= other->getZ();
}

bool Vector3::isEqual(Vector3 *other) {
	bool XEquality = (x >= other->getX() - TOLERANCE
			&& x <= other->getX() + TOLERANCE);
	bool YEquality = (y >= other->getY() - TOLERANCE
			&& y <= other->getY() + TOLERANCE);
	bool ZEquality = (z >= other->getZ() - TOLERANCE
			&& z <= other->getZ() + TOLERANCE);

	return (XEquality && YEquality && ZEquality);
}

float Vector3::getAngle() {
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

Vector3& Vector3::operator=(Vector3 &other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Vector3& vect) {
	out << vect.x << " " << vect.y << " " << vect.z;
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Vector3& vect) {
	float x, y, z;
	in >> x;
	in >> y;
	in >> z;
	vect.setValues(x, y, z);
	return in;
}
