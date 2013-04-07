/* 
 * File:   Posicion.h
 * Author: lucas
 *
 * Created on March 21, 2013, 10:58 AM
 */

#ifndef POSITION_H
#define	POSITION_H

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

private:
	int x, y, z;
};

#endif	/* POSITION_H */

