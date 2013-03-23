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
    Position(int x, int y, int z);
    void changeTo(int nuevaX, int nuevaY, int nuevaZ);
private:
    Position();
    int x, y, z;
};

#endif	/* POSITION_H */

