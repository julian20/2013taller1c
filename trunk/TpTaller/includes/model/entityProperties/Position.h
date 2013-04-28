/* 
 * File:   Posicion.h
 * Author: lucas
 *
 * Created on March 21, 2013, 10:58 AM
 */

#ifndef POSITION_H
#define	POSITION_H

#include <iostream>

using namespace std;

class Position {
public:
	Position();
	Position(int x, int y, int z);
	Position(int coordX, int coordY);
	void changeTo(int nuevaX, int nuevaY, int nuevaZ);
	int getX() const;
	void setX(int x);
	int getY() const;
	void setY(int y);
	int getZ() const;
	void setZ(int z);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const Position&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, Position&);

	Position& operator=(Position &other);

private:
	int x, y, z;
};

#endif	/* POSITION_H */

