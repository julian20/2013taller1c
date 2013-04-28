/*
 * Base.cpp
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#include <model/entityProperties/Base.h>

Base::Base() {
	this->anchorPixel = new Vector2(0,0);
	this->length = 1;
	this->width = 1;
}

Base::~Base() {
	delete anchorPixel;
}

unsigned int Base::getLength() {
	return this->length;
}

void Base::setLength(unsigned int length) {
	this->length = length;
}

unsigned int Base::getWidth() {
	return this->width;
}

void Base::setWidth(unsigned int width) {
	this->width = width;
}

Vector2* Base::getAnchorPixel() {
	return this->anchorPixel;
}

void Base::setAnchorPixel(Vector2* anchorPixel) {
	this->anchorPixel = anchorPixel;
}

Base& Base::operator=(Base &other){
	this->length = other.length;
	this->width = other.width;
	*(this->anchorPixel) = *(other.anchorPixel);
	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Base& base){
	out << base.width << " " << base.length << " " << *(base.anchorPixel);
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Base& base){
	int width, length;
	Vector2* anchorPixel = new Vector2(0,0);
	in >> width;
	in >> length;
	in >> *anchorPixel;
	base.setWidth(width);
	base.setLength(length);
	base.setAnchorPixel(anchorPixel);
	return in;

}
