/* 
 * File:   Position.cpp
 * Author: lucas
 * 
 * Created on March 21, 2013, 10:58 AM
 */

#include <model/entityProperties/position/Position.h>

Position::Position(){
	x=0;
	y=0;
	z=0;
}

Position::Position(int coordX, int coordY, int coordZ) {
    x = coordX;
    y = coordY;
    z = coordZ;

}

void Position::changeTo(int newX, int newY, int newZ) {
    x = newX;
    y = newY;
    z = newZ;

}

