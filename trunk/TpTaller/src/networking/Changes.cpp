/*
 * GlobalChanges.cpp
 *
 *  Created on: 24/04/2013
 *      Author: tomas
 */

#include <networking/Changes.h>
#include <iostream>

namespace std {

Changes::Changes() {
	// TODO Auto-generated constructor stub

}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, Changes& c){

	return out;
}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, Changes& c){

	return in;
}


Changes::~Changes() {

}

} /* namespace std */
