/*
 * Vector.h
 *
 *  Created on: Mar 27, 2013
 *      Author: fede
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

#define _USE_MATH_DEFINES	// To use math.h constants
#include <math.h>

class Vector3 {
public:
	Vector3(float _x = 0, float _y = 0, float _z = 0);
	void setValues(float _x, float _y, float _z = 0);
	float getX();
	float getY();
	float getZ();
	void normalize();
	float getNorm();
	void multiplyBy(float val);
	void add(Vector3 *other);
	void sub(Vector3 *other);
	bool isEqual(Vector3 *other);
	float getAngle();
private:
	float x, y, z;
};

#endif /* VECTOR_H_ */
