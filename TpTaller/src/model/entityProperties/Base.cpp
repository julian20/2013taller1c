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
