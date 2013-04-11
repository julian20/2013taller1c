/*
 * Base.h
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#ifndef BASE_H_
#define BASE_H_

#include <model/Vector2.h>

class Base {
public:
	Base();
	virtual ~Base();
	unsigned int getWidth();
	unsigned int getLength();
	Vector2* getAnchorPixel();
	void setWidth(unsigned int width);
	void setLength(unsigned int length);
	void setAnchorPixel(Vector2* anchorPixel);
private:
	unsigned int width;
	unsigned int length;
	Vector2* anchorPixel;
};

#endif /* BASE_H_ */