/*
 * Coordinates.h
 *
 *  Created on: Apr 2, 2013
 *      Author: fede
 */

#ifndef COORDINATES_H_
#define COORDINATES_H_

#include <iostream>

using namespace std;

class Coordinates {
public:
	Coordinates();
	Coordinates(int _row, int _col);
	Coordinates(const Coordinates &coords);
	void changeTo(int _row, int _col);
	int getRow() const;
	void setRow(int _row);
	int getCol() const;
	void setCol(int _col);
	bool isEqual(Coordinates coords);
	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const Coordinates&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, Coordinates&);

	Coordinates& operator=(Coordinates &other);

private:
	int row, col;
};

#endif /* COORDINATES_H_ */
