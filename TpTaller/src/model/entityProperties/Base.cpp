/*
 * Base.cpp
 *
 *  Created on: Apr 4, 2013
 *      Author: gonchub
 */

#include <model/entityProperties/Base.h>

Base::Base() {
	this->anchorPixel = new Vector2(0, 0);
	this->cols = 1;
	this->rows = 1;
}

Base::~Base() {
	delete anchorPixel;
}

int Base::getCols() {
	return this->cols;
}

void Base::setCols(int _cols) {
	this->cols = _cols;
}

int Base::getRows() {
	return this->rows;
}

void Base::setRows(int _rows) {
	this->rows = _rows;
}

Vector2* Base::getAnchorPixel() {
	return this->anchorPixel;
}

void Base::setAnchorPixel(Vector2* anchorPixel) {
	this->anchorPixel = anchorPixel;
}

Base& Base::operator=(Base &other) {
	this->cols = other.cols;
	this->rows = other.rows;
	*(this->anchorPixel) = *(other.anchorPixel);
	return *this;
}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, const Base& base) {
	out << base.rows << " " << base.cols << " " << *(base.anchorPixel);
	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Base& base) {
	unsigned int width, length;
	Vector2* anchorPixel = new Vector2(0, 0);
	in >> width;
	in >> length;
	in >> *anchorPixel;
	base.setRows(width);
	base.setCols(length);
	base.setAnchorPixel(anchorPixel);
	return in;

}
