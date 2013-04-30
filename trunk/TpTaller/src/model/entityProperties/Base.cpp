/*
 * Base.cpp
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#include <model/entityProperties/Base.h>

Base::Base() {
	this->anchorPixel = new Vector2(0,0);
	this->height = 1;
	this->width = 1;
}

Base::~Base() {
	delete anchorPixel;
}

int Base::getHeight() {
	return this->height;
}

void Base::setHeight(int _height) {
	this->height = _height;
}

int Base::getWidth() {
	return this->width;
}

void Base::setWidth(int width) {
	this->width = width;
}

Vector2* Base::getAnchorPixel() {
	return this->anchorPixel;
}

void Base::setAnchorPixel(Vector2* anchorPixel) {
	this->anchorPixel = anchorPixel;
}

Base& Base::operator=(Base &other){
	this->height = other.height;
	this->width = other.width;
	*(this->anchorPixel) = *(other.anchorPixel);
	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Base& base){
	out << base.width << " " << base.height << " " << *(base.anchorPixel);
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Base& base){
	unsigned int width, length;
	Vector2* anchorPixel = new Vector2(0,0);
	in >> width;
	in >> length;
	in >> *anchorPixel;
	base.setWidth(width);
	base.setHeight(length);
	base.setAnchorPixel(anchorPixel);
	return in;

}
