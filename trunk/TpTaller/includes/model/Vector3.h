/*
 * Vector.h
 *
 *  Created on: Mar 27, 2013
 *      Author: fede
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#define _USE_MATH_DEFINES	// To use math.h constants
#include <math.h>

class Vector3 {
public:
	Vector3(float _x = 0, float _y = 0, float _z = 0);
	void SetValues(float _x, float _y, float _z = 0);
	float GetX();
	float GetY();
	float GetZ();
	void Normalize();
	float GetNorm();
	void MultiplyBy(float val);
	void Add(Vector3 *other);
	void Sub(Vector3 *other);
	bool IsEqual(Vector3 *other);
	float GetAngle();
private:
	float x, y, z;
};

#endif /* VECTOR_H_ */
