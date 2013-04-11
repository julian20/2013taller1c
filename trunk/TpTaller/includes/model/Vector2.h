/*
 * Vector.h
 *
 *  Created on: Mar 27, 2013
 *      Author: fede
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_

#define _USE_MATH_DEFINES	// To use math.h constants
#include <math.h>

class Vector2 {
public:
	Vector2(float _x = 0, float _y = 0);
	void setValues(float _x, float _y);
	float getX();
	float getY();
	void normalize();
	float getNorm();
	void multiplyBy(float val);
	void add(Vector2 *other);
	void sub(Vector2 *other);
	bool isEqual(Vector2 *other);
	float getAngle();
private:
	float x, y;
};

#endif /* VECTOR_H_ */
