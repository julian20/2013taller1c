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

class Vector2{
public:
	Vector2(float _x = 0, float _y = 0);
	void SetValues(float _x, float _y);
	float GetX();
	float GetY();
	void Normalize();
	float GetNorm();
	void MultiplyBy(float val);
	void Add(Vector2 *other);
	void Sub(Vector2 *other);
	bool IsEqual(Vector2 *other);
	float GetAngle();
private:
	float x, y;
};


#endif /* VECTOR_H_ */
