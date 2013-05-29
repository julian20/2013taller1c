/*
 * Speed.h
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#ifndef SPEED_H_
#define SPEED_H_

#include <model/Vector2.h>

using namespace std;

class Speed {
public:
	Speed();
	virtual ~Speed();
	Speed(int magnitude, Vector2* direction);
	int getMagnitude();
	void setMagnitude(int magnitude);
	Vector2* getDirection();
	void setDirection(Vector2* direction);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const Speed&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, Speed&);

	Speed& operator=(Speed &other);

private:

	Vector2* direction;
	int magnitude;
};

#endif /* SPEED_H_ */
